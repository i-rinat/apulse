/*
 * Copyright © 2014-2015  Rinat Ibragimov
 *
 * This file is part of "apulse" project.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "apulse.h"
#include "trace.h"
#include "util.h"

#define MAKE_SND_LIB_VERSION(a,b,c)  (((a)<<16)|((b)<<8)|(c))

#define HAVE_SND_PCM_AVAIL  SND_LIB_VERSION >= MAKE_SND_LIB_VERSION(1, 0, 18)

static
void
deh_stream_state_changed(pa_mainloop_api *api, pa_defer_event *de, void *userdata)
{
    pa_stream *s = userdata;
    if (s->state_cb)
        s->state_cb(s, s->state_cb_userdata);
    pa_stream_unref(s);
}

static
void
deh_stream_first_readwrite_callback(pa_mainloop_api *api, pa_defer_event *de, void *userdata)
{
    pa_stream *s = userdata;

    if (s->direction == PA_STREAM_PLAYBACK) {
        size_t writable_size = pa_stream_writable_size(s);
        if (s->write_cb && writable_size > 0)
            s->write_cb(s, writable_size, s->write_cb_userdata);
    } else if (s->direction == PA_STREAM_RECORD) {
        size_t readable_size = pa_stream_readable_size(s);
        if (s->read_cb && readable_size > 0)
            s->read_cb(s, readable_size, s->read_cb_userdata);
    }
    pa_stream_unref(s);
}

static
void
data_available_for_stream(pa_mainloop_api *a, pa_io_event *ioe, int fd, pa_io_event_flags_t events,
                          void *userdata)
{
    pa_stream          *s = userdata;
    snd_pcm_sframes_t   frame_count;
    size_t              frame_size = pa_frame_size(&s->ss);
    char                buf[16 * 1024];
    const size_t        buf_size = pa_find_multiple_of(sizeof(buf), frame_size, 0);
    int                 paused = g_atomic_int_get(&s->paused);

    if (events & (PA_IO_EVENT_INPUT | PA_IO_EVENT_OUTPUT)) {

#if HAVE_SND_PCM_AVAIL
        frame_count = snd_pcm_avail(s->ph);
#else
        snd_pcm_hwsync(s->ph);
        frame_count = snd_pcm_avail_update(s->ph);
#endif

        if (frame_count < 0) {
            if (frame_count == -EBADFD) {
                // stream was closed
                return;
            }

            int cnt = 0, ret;
            do {
                cnt ++;
                ret = snd_pcm_recover(s->ph, frame_count, 1);
            } while (ret == -1 && errno == EINTR && cnt < 5);

            switch (snd_pcm_state(s->ph)) {
            case SND_PCM_STATE_OPEN:
                // Highly unlikely device will be here in this state. But if it is, there is nothing
                // can be done.
                trace_error(
                    "Stream '%s' of context '%s' have its associated PCM device in "
                    "SND_PCM_STATE_OPEN state. Reconfiguration is required, but is not possible at "
                    "the moment. Giving up.",
                    s->name ? s->name : "", s->c->name ? s->c->name : "");
                break;

            case SND_PCM_STATE_SETUP:
                // There is configuration, but device is not prepared and not started.
                snd_pcm_prepare(s->ph);
                snd_pcm_start(s->ph);
                break;

            case SND_PCM_STATE_PREPARED:
                // Device prepared, but not started.
                snd_pcm_start(s->ph);
                break;

            case SND_PCM_STATE_RUNNING:
                // That's the expected state.
                break;

            case SND_PCM_STATE_XRUN:
                trace_error(
                    "Stream '%s' of context '%s' have its associated device in SND_PCM_STATE_XRUN "
                    "state even after xrun recovery.",
                    s->name ? s->name : "", s->c->name ? s->c->name : "");
                break;

            case SND_PCM_STATE_DRAINING:
                trace_error(
                    "Stream '%s' of context '%s' have its associated device in "
                    "SND_PCM_STATE_DRAINING state, which is highly unusual.",
                    s->name ? s->name : "", s->c->name ? s->c->name : "");
                break;

            case SND_PCM_STATE_PAUSED:
                // Resume from paused state.
                snd_pcm_pause(s->ph, 0);
                break;

            case SND_PCM_STATE_SUSPENDED:
                // Resume from suspended state.
                snd_pcm_resume(s->ph);
                break;

            case SND_PCM_STATE_DISCONNECTED:
                trace_error(
                    "Stream '%s' of context '%s' have its associated device in "
                    "SND_PCM_STATE_DISCONNECTED state. Giving up.",
                    s->name ? s->name : "", s->c->name ? s->c->name : "");
                break;
            }

#if HAVE_SND_PCM_AVAIL
            frame_count = snd_pcm_avail(s->ph);
#else
            snd_pcm_hwsync(s->ph);
            frame_count = snd_pcm_avail_update(s->ph);
#endif

            if (frame_count < 0) {
                trace_error("%s, can't recover after failed snd_pcm_avail (%d)\n", __func__,
                            (int)frame_count);
                return;
            }
        }
    } else {
        return;
    }

    if (events & PA_IO_EVENT_OUTPUT) {
        if (paused) {
            // client stream is corked. Pass silence to ALSA
            size_t bytecnt = MIN(buf_size, frame_count * frame_size);
            memset(buf, 0, bytecnt);
            snd_pcm_writei(s->ph, buf, bytecnt / frame_size);
        } else {
            size_t writable_size = pa_stream_writable_size(s);

            // Ask client for data, but only if we are ready for at least |minreq| bytes.
            if (s->write_cb && writable_size >= s->buffer_attr.minreq)
                s->write_cb(s, s->buffer_attr.minreq, s->write_cb_userdata);

            size_t bytecnt = MIN(buf_size, frame_count * frame_size);
            bytecnt = ringbuffer_read(s->rb, buf, bytecnt);

            pa_apply_volume_multiplier(buf, bytecnt, s->volume, &s->ss);

            if (bytecnt == 0) {
                // application is not ready yet, play silence
                bytecnt = MIN(buf_size, frame_count * frame_size);
                memset(buf, 0, bytecnt);
            }
            snd_pcm_writei(s->ph, buf, bytecnt / frame_size);
        }
    }

    if (events & PA_IO_EVENT_INPUT) {
        if (paused) {
            // client stream is corked. Read data from ALSA and discard them
            size_t bytecnt = MIN(buf_size, frame_count * frame_size);
            snd_pcm_readi(s->ph, buf, bytecnt / frame_size);
        } else {
            size_t bytecnt = ringbuffer_writable_size(s->rb);

            if (bytecnt == 0) {
                // ringbuffer is full because app doesn't read data fast enough.
                // Make some room
                ringbuffer_drop(s->rb, frame_count * frame_size);
                bytecnt = ringbuffer_writable_size(s->rb);
            }

            bytecnt = MIN(bytecnt, frame_count * frame_size);
            bytecnt = MIN(bytecnt, buf_size);

            if (bytecnt > 0) {
                snd_pcm_readi(s->ph, buf, bytecnt / frame_size);
                pa_apply_volume_multiplier(buf, bytecnt, s->c->source_volume, &s->ss);
                ringbuffer_write(s->rb, buf, bytecnt);
            }

            size_t readable_size = pa_stream_readable_size(s);
            if (s->read_cb && readable_size > 0)
                s->read_cb(s, readable_size, s->read_cb_userdata);
        }
    }
}

static
int
do_connect_pcm(pa_stream *s, snd_pcm_stream_t stream_direction)
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    int errcode;
    const char *device_name;
    const char *direction_name;

    switch (stream_direction) {
    default:
    case SND_PCM_STREAM_PLAYBACK:
        device_name = getenv("APULSE_PLAYBACK_DEVICE");
        direction_name = "playback";
        break;
    case SND_PCM_STREAM_CAPTURE:
        device_name = getenv("APULSE_CAPTURE_DEVICE");
        direction_name = "capture";
        break;
    }

    if (device_name == NULL)
        device_name = "default";

    char *device_description = g_strdup_printf("%s device \"%s\"", direction_name, device_name);
    if (!device_description) {
        trace_error("%s: can't allocate memory for device description string\n", __func__);
        goto fatal_error;
    }

    errcode = snd_pcm_open(&s->ph, device_name, stream_direction, 0);
    if (errcode != 0) {
        trace_error("%s: can't open %s. Error code %d (%s)\n", __func__, device_description,
                    errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    errcode = snd_pcm_hw_params_malloc(&hw_params);
    if (errcode != 0) {
        trace_error("%s: can't allocate memory for hw parameters for %s. Error code %d (%s)\n",
                    __func__, device_description, errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    errcode = snd_pcm_hw_params_any(s->ph, hw_params);
    if (errcode != 0) {
        trace_error("%s: can't get initial hw parameters for %s. Error code %d (%s)\n", __func__,
                    device_description, errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    errcode = snd_pcm_hw_params_set_access(s->ph, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (errcode != 0) {
        trace_error("%s: can't select interleaved mode for %s. Error code %d (%s)\n", __func__,
                    device_description, errcode, snd_strerror(errcode));
        // TODO: is it worth to support non-interleaved mode?
        goto fatal_error;
    }

    errcode = snd_pcm_hw_params_set_format(s->ph, hw_params, pa_format_to_alsa(s->ss.format));
    if (errcode != 0) {
        snd_pcm_format_t alsa_format = pa_format_to_alsa(s->ss.format);
        trace_error("%s: can't set sample format %d (\"%s\") for %s. Error code %d (%s)\n",
                    __func__, alsa_format, snd_pcm_format_name(alsa_format), device_description,
                    errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    errcode = snd_pcm_hw_params_set_rate_resample(s->ph, hw_params, 1);
    if (errcode != 0) {
        trace_error("%s: can't enable rate resample for %s. Error code %d (%s)\n", __func__,
                    device_description, errcode, snd_strerror(errcode));
        // This is not a fatal error. Audio speed will be wrong, but there will be something.
        // And it sounds funny.
    }

    unsigned int rate = s->ss.rate;
    int dir = 0;

    errcode = snd_pcm_hw_params_set_rate_near(s->ph, hw_params, &rate, &dir);
    if (errcode != 0) {
        trace_error("%s: can't set sample rate for %s. Error code %d (%s)\n", __func__,
                    device_description, errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    trace_info_f("%s: demanded %d Hz sample rate, got %d Hz for %s, dir = %d\n", __func__,
                 (int)s->ss.rate, (int)rate, device_description, dir);

    if (rate != s->ss.rate)
        trace_error("%s: actual sample rate, %d Hz, differs from required %d Hz\n", __func__,
                    (int)rate, (int)s->ss.rate);

    errcode = snd_pcm_hw_params_set_channels(s->ph, hw_params, s->ss.channels);
    if (errcode != 0) {
        trace_error("%s: can't set channel count to %d for %s. Error code %d (%s)\n", __func__,
                    (int)s->ss.channels, device_description, errcode, snd_strerror(errcode));
        // TODO: channel count handling?
        goto fatal_error;
    }

    const size_t frame_size = pa_frame_size(&s->ss);
    snd_pcm_uframes_t requested_period_size = s->buffer_attr.minreq / frame_size;
    snd_pcm_uframes_t period_size = requested_period_size;
    dir = 1;  // Prefer larger period sizes, if exact is not possible.
    errcode = snd_pcm_hw_params_set_period_size_near(s->ph, hw_params, &period_size, &dir);
    if (errcode != 0) {
        trace_error("%s: can't set period size to %d frames for %s. Error code %d (%s)\n", __func__,
                    (int)requested_period_size, device_description, errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    trace_info_f("%s: requested period size of %d frames, got %d frames for %s\n", __func__,
                 (int)requested_period_size, (int)period_size, device_description);

    // Set up buffer size. Ensure it's at least four times larger than a period size.
    snd_pcm_uframes_t requested_buffer_size = s->buffer_attr.tlength / frame_size;
    snd_pcm_uframes_t buffer_size = MAX(requested_buffer_size, 4 * period_size);
    errcode = snd_pcm_hw_params_set_buffer_size_near(s->ph, hw_params, &buffer_size);
    if (errcode != 0) {
        trace_error(
            "%s: can't set buffer size to %d frames for %s. Error code %d (%s)\n",
            __func__, (int)buffer_size, device_description, errcode, snd_strerror(errcode));
        goto fatal_error;
    }

    trace_info_f("%s: requested buffer size of %d frames, got %d frames for %s\n", __func__,
                 (int)requested_buffer_size, (int)buffer_size, device_description);

    errcode = snd_pcm_hw_params(s->ph, hw_params);
    if (errcode != 0) {
        trace_error("%s: can't apply configured hw parameter block for %s\n", __func__,
                    device_description);
        goto fatal_error;
    }

    snd_pcm_hw_params_free(hw_params);

    errcode = snd_pcm_sw_params_malloc(&sw_params);
    if (errcode != 0) {
        trace_error("%s: can't allocate memory for sw parameters for %s\n", __func__,
                    device_description);
        goto fatal_error;
    }

    errcode = snd_pcm_sw_params_current(s->ph, sw_params);
    if (errcode != 0) {
        trace_error("%s: can't acquire current sw parameters for %s\n", __func__,
                    device_description);
        goto fatal_error;
    }

    errcode = snd_pcm_sw_params_set_avail_min(s->ph, sw_params, period_size);
    if (errcode != 0) {
        trace_error("%s: can't set avail min for %s\n", __func__, device_description);
        goto fatal_error;
    }

    // no period event requested

    errcode = snd_pcm_sw_params(s->ph, sw_params);
    if (errcode != 0) {
        trace_error("%s: can't apply sw parameters for %s\n", __func__, device_description);
        goto fatal_error;
    }

    snd_pcm_sw_params_free(sw_params);

    errcode = snd_pcm_prepare(s->ph);
    if (errcode != 0) {
        trace_error("%s: can't prepare PCM device to use for %s\n", __func__, device_description);
        goto fatal_error;
    }

    int nfds = snd_pcm_poll_descriptors_count(s->ph);
    struct pollfd *fds = calloc(nfds, sizeof(struct pollfd));
    s->ioe = calloc(nfds, sizeof(pa_io_event *));
    s->nioe = nfds;
    snd_pcm_poll_descriptors(s->ph, fds, nfds);
    for (int k = 0; k < nfds; k ++) {
        pa_mainloop_api *api = s->c->mainloop_api;
        s->ioe[k] = api->io_new(api, fds[k].fd, 0x80000000 | fds[k].events,
                                data_available_for_stream, s);
        s->ioe[k]->pcm = s->ph;
    }
    free(fds);

    s->state = PA_STREAM_READY;
    pa_stream_ref(s);
    s->c->mainloop_api->defer_new(s->c->mainloop_api, deh_stream_state_changed, s);
    pa_stream_ref(s);
    s->c->mainloop_api->defer_new(s->c->mainloop_api, deh_stream_first_readwrite_callback, s);

    g_free(device_description);
    return 0;

fatal_error:
    trace_error(
        "%s: failed to open ALSA device. Apulse does no resampling or format conversion, leaving "
        "that task to ALSA plugins. Ensure that selected device is capable of playing a particular "
        "sample format at a particular rate. They have to be supported by either hardware "
        "directly, or by \"plug\" and \"dmix\" ALSA plugins which will perform required "
        "conversions on CPU.\n",
        __func__);

    g_free(device_description);
    return -1;
}

APULSE_EXPORT
int
pa_stream_begin_write(pa_stream *p, void **data, size_t *nbytes)
{
    trace_info_f("F %s p=%p nbytes=%p(%" PRIu64 ")\n", __func__, p, nbytes,
                 (uint64_t)(nbytes ? *nbytes : 0));

    free(p->write_buffer);

    if (*nbytes == (size_t)-1)
        *nbytes = 8192;

    *nbytes = pa_find_multiple_of(*nbytes, pa_frame_size(&p->ss), 0);

    p->write_buffer = malloc(*nbytes);

    if (!p->write_buffer)
        return -1;

    *data = p->write_buffer;

    return 0;
}

APULSE_EXPORT
int
pa_stream_cancel_write(pa_stream *p)
{
    trace_info_f("F %s p=%p\n", __func__, p);

    free(p->write_buffer);
    p->write_buffer = NULL;

    return 0;
}

static void
stream_adjust_buffer_attrs(pa_stream *s, const pa_buffer_attr *attr)
{
    pa_buffer_attr *ba = &s->buffer_attr;
    const size_t frame_size = pa_frame_size(&s->ss);

    if (attr) {
        *ba = *attr;
    } else {
        // If client passed NULL, all parameters have default values.
        ba->maxlength = (uint32_t)-1;
        ba->tlength = (uint32_t)-1;
        ba->prebuf = (uint32_t)-1;
        ba->minreq = (uint32_t)-1;
        ba->fragsize = (uint32_t)-1;
    }

    // Adjust default values.
    // Overall buffer length.
    if (ba->maxlength == (uint32_t)-1)
        ba->maxlength = 4 * 1024 * 1024;

    if (ba->maxlength == 0)
        ba->maxlength = frame_size;

    // Target length of a buffer.
    if (ba->tlength == (uint32_t)-1)
        ba->tlength = pa_usec_to_bytes(2 * 1000 * 1000, &s->ss);

    if (ba->tlength == 0)
        ba->tlength = frame_size;

    ba->tlength = MIN(ba->tlength, ba->maxlength);

    // Minimum request (playback).
    if (ba->minreq == (uint32_t)-1) {
        ba->minreq = pa_usec_to_bytes(20 * 1000, &s->ss);
        ba->minreq = MIN(ba->minreq, ba->tlength / 4);
    }

    if (ba->minreq == 0)
        ba->minreq = frame_size;

    // Fragment size (recording).
    if (ba->fragsize == (uint32_t)-1) {
        ba->fragsize = pa_usec_to_bytes(20 * 1000, &s->ss);
    }

    if (ba->fragsize == 0)
        ba->fragsize = frame_size;

    // Pre-buffering.
    if (ba->prebuf == (uint32_t)-1)
        ba->prebuf = ba->tlength - ba->minreq;

    if (ba->prebuf > ba->tlength - ba->minreq)
        ba->prebuf = ba->tlength - ba->minreq;

    // Ensure values are all multiple of |frame_size|.
    ba->maxlength = pa_find_multiple_of(ba->maxlength, frame_size, 1);
    ba->tlength = pa_find_multiple_of(ba->tlength, frame_size, 1);
    ba->prebuf = pa_find_multiple_of(ba->prebuf, frame_size, 1);
    ba->minreq = pa_find_multiple_of(ba->minreq, frame_size, 1);
    ba->fragsize = pa_find_multiple_of(ba->fragsize, frame_size, 1);
}

APULSE_EXPORT
int
pa_stream_connect_playback(pa_stream *s, const char *dev, const pa_buffer_attr *attr,
                           pa_stream_flags_t flags, const pa_cvolume *volume,
                           pa_stream *sync_stream)
{
    gchar *s_attr = trace_pa_buffer_attr_as_string(attr);
    trace_info_f("P %s s=%p, dev=%s, attr=%s, flags=0x%x, volume=%p, sync_stream=%p\n", __func__,
               s, dev, s_attr, flags, volume, sync_stream);
    g_free(s_attr);

    s->direction = PA_STREAM_PLAYBACK;
    stream_adjust_buffer_attrs(s, attr);

    if (do_connect_pcm(s, SND_PCM_STREAM_PLAYBACK) != 0)
        goto err;

    g_atomic_int_set(&s->paused, !!(flags & PA_STREAM_START_CORKED));

    return 0;
err:
    return -1;
}

static void
pa_stream_cork_impl(pa_operation *op)
{
    g_atomic_int_set(&op->s->paused, !!(op->int_arg_1));

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_cork(pa_stream *s, int b, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, b=%d, cb=%p, userdata=%p\n", __func__, s, b, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_cork_impl);
    op->s = s;
    op->int_arg_1 = b;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
int
pa_stream_disconnect(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    if (s->state != PA_STREAM_READY)
        return PA_ERR_BADSTATE;

    for (int k = 0; k < s->nioe; k ++) {
        pa_mainloop_api *api = s->c->mainloop_api;
        api->io_free(s->ioe[k]);
    }
    free(s->ioe);

    snd_pcm_close(s->ph);
    s->state = PA_STREAM_TERMINATED;

    return PA_OK;
}

static void
pa_stream_drain_impl(pa_operation *op)
{
    snd_pcm_drain(op->s->ph);

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_drain(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_drain_impl);
    op->s = s;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

static void
pa_stream_flush_impl(pa_operation *op)
{
    // TODO: is it ok to do nothing?

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_flush(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_flush_impl);
    op->s = s;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
uint32_t
pa_stream_get_index(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    return s->idx;
}

APULSE_EXPORT
int
pa_stream_get_latency(pa_stream *s, pa_usec_t *r_usec, int *negative)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    snd_pcm_sframes_t delay;

    if (snd_pcm_delay(s->ph, &delay) != 0)
        delay = 0;

    if (r_usec)
        *r_usec = 1000 * 1000 * delay / s->ss.rate;
    if (negative)
        *negative = 0;
    return 0;
}

APULSE_EXPORT
const pa_sample_spec *
pa_stream_get_sample_spec(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    return &s->ss;
}

APULSE_EXPORT
pa_stream_state_t
pa_stream_get_state(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    return s->state;
}

APULSE_EXPORT
int
pa_stream_get_time(pa_stream *s, pa_usec_t *r_usec)
{
    trace_info_f("F %s\n", __func__);

    // TODO: handle playback/capture delays?
    int64_t data_index = s->timing_info.write_index;
    if (data_index < 0)
        data_index = 0;

    *r_usec = pa_bytes_to_usec(data_index, &s->ss);
    return 0;
}

APULSE_EXPORT
const pa_timing_info *
pa_stream_get_timing_info(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    snd_pcm_sframes_t delay;

    if (snd_pcm_delay(s->ph, &delay) != 0)
        delay = 0;
    s->timing_info.read_index = s->timing_info.write_index - delay * pa_frame_size(&s->ss);

    return &s->timing_info;
}

APULSE_EXPORT
int
pa_stream_is_corked(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);
    return g_atomic_int_get(&s->paused);
}

APULSE_EXPORT
int
pa_stream_is_suspended(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);
    // ALSA sink is never suspended
    return 0;
}

APULSE_EXPORT
pa_stream *
pa_stream_new(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map)
{
    trace_info_f("F %s c=%p, name=%s, ss=%p, map=%p\n", __func__, c, name, ss, map);

    pa_proplist *p = pa_proplist_new();
    pa_stream *s = pa_stream_new_with_proplist(c, name, ss, map, p);
    pa_proplist_free(p);
    return s;
}

APULSE_EXPORT
pa_stream *
pa_stream_new_extended(pa_context *c, const char *name, pa_format_info *const *formats,
                       unsigned int n_formats, pa_proplist *p)
{
    trace_info_f("P %s c=%p, name=%s, formats=%p, n_formats=%u, p=%p\n", __func__, c, name,
               formats, n_formats, p);

    // TODO: multiple formats?

    // take first format
    if (n_formats < 1) {
        trace_error("%s, no formats\n", __func__);
        return NULL;
    }

    pa_sample_spec ss = {
        .format =   PA_SAMPLE_S16LE,
        .rate =     48000,
        .channels = 2,
    };

    const char *val;

    val = pa_proplist_gets(formats[0]->plist, PA_PROP_FORMAT_SAMPLE_FORMAT);
    if (val)
        ss.format = pa_sample_format_from_string(val);

    val = pa_proplist_gets(formats[0]->plist, PA_PROP_FORMAT_RATE);
    if (val)
        ss.rate = atoi(val);

    val = pa_proplist_gets(formats[0]->plist, PA_PROP_FORMAT_CHANNELS);
    if (val)
        ss.channels = atoi(val);

    return pa_stream_new_with_proplist(c, name, &ss, NULL, p);
}

APULSE_EXPORT
pa_stream *
pa_stream_new_with_proplist(pa_context *c, const char *name, const pa_sample_spec *ss,
                            const pa_channel_map *map, pa_proplist *p)
{
    trace_info_f("F %s c=%p, name=%s, ss={.format=%d, .rate=%u, .channels=%u}, map=%p, p=%p\n",
               __func__, c, name, ss->format, ss->rate, ss->channels, map, p);

    pa_stream *s = calloc(1, sizeof(pa_stream));
    s->c = c;
    s->ref_cnt = 1;
    s->state = PA_STREAM_UNCONNECTED;
    s->ss = *ss;

    s->idx = c->next_stream_idx ++;
    g_hash_table_insert(c->streams_ht, GINT_TO_POINTER(s->idx), s);

    // fill initial values of s->timing_info
    gettimeofday(&s->timing_info.timestamp, NULL);
    s->timing_info.synchronized_clocks = 1;
    s->timing_info.sink_usec = 0;
    s->timing_info.source_usec = 0;
    s->timing_info.transport_usec = 0;
    s->timing_info.playing = 1;
    s->timing_info.write_index_corrupt = 0;
    s->timing_info.write_index = 0;
    s->timing_info.read_index_corrupt = 0;
    s->timing_info.read_index = 0;
    s->timing_info.configured_sink_usec = 0;
    s->timing_info.configured_source_usec = 0;
    s->timing_info.since_underrun = 0;

    s->rb = ringbuffer_new(72 * 1024);    // TODO: figure out size
    s->peek_buffer = malloc(s->rb->end - s->rb->start);

    for (uint32_t k = 0; k < PA_CHANNELS_MAX; k++)
        s->volume[k] = PA_VOLUME_NORM;

    return s;
}

APULSE_EXPORT
pa_stream *
pa_stream_ref(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    s->ref_cnt ++;
    return s;
}

APULSE_EXPORT
void
pa_stream_set_latency_update_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->latency_update_cb = cb;
    s->latency_update_cb_userdata = userdata;
}

static void
pa_stream_set_name_impl(pa_operation *op)
{
    free(op->s->name);
    op->s->name = op->char_ptr_arg_1;

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_set_name(pa_stream *s, const char *name, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("P %s s=%p, name=%s, cb=%p, userdata=%p\n", __func__, s, name, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_set_name_impl);
    op->s = s;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;
    op->char_ptr_arg_1 = strdup(name ? name : "");

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
void
pa_stream_set_state_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->state_cb = cb;
    s->state_cb_userdata = userdata;
}

APULSE_EXPORT
void
pa_stream_set_write_callback(pa_stream *s, pa_stream_request_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->write_cb = cb;
    s->write_cb_userdata = userdata;
}

static void
pa_stream_trigger_impl(pa_operation *op)
{
    // TODO: does nothing?

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_trigger(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_trigger_impl);
    op->s = s;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
void
pa_stream_unref(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    s->ref_cnt --;
    if (s->ref_cnt == 0) {
        g_hash_table_remove(s->c->streams_ht, GINT_TO_POINTER(s->idx));
        ringbuffer_free(s->rb);
        free(s->peek_buffer);
        free(s->write_buffer);
        free(s->name);
        free(s);
    }
}

static void
pa_stream_update_timing_info_impl(pa_operation *op)
{
    gettimeofday(&op->s->timing_info.timestamp, NULL);

    if (op->s->latency_update_cb)
        op->s->latency_update_cb(op->s, op->s->latency_update_cb_userdata);

    if (op->stream_success_cb)
        op->stream_success_cb(op->s, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_stream_update_timing_info(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    pa_operation *op = pa_operation_new(s->c->mainloop_api, pa_stream_update_timing_info_impl);
    op->s = s;
    op->stream_success_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
size_t
pa_stream_writable_size(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    size_t writable_size = ringbuffer_writable_size(s->rb);

    // Some applications try to push more data than reported to be available
    // by pa_stream_writable_size(), which is fine for original PulseAudio
    // but is a severe error in this implementation, since buffer size is
    // limited.
    //
    // Workaround issue by reserving certain amount for that case.

    const size_t limit = 16 * 1024; // TODO: adaptive values?

    if (writable_size < limit)
        writable_size = 0;

    return pa_find_multiple_of(writable_size, pa_frame_size(&s->ss), 0);
}

APULSE_EXPORT
size_t
pa_stream_readable_size(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    size_t readable_size = ringbuffer_readable_size(s->rb);
    return pa_find_multiple_of(readable_size, pa_frame_size(&s->ss), 0);
}

APULSE_EXPORT
int
pa_stream_write(pa_stream *s, const void *data, size_t nbytes, pa_free_cb_t free_cb, int64_t offset,
                pa_seek_mode_t seek)
{
    trace_info_f("F %s s=%p, data=%p, nbytes=%zu, free_cb=%p, offset=%"PRId64", seek=%u\n", __func__,
               s, data, nbytes, free_cb, offset, seek);

    if (offset != 0)
        trace_error("%s, offset != 0\n", __func__);
    if (seek != PA_SEEK_RELATIVE)
        trace_error("%s, seek != PA_SEEK_RELATIVE\n", __func__);

    size_t written = ringbuffer_write(s->rb, data, nbytes);
    s->timing_info.since_underrun += written;
    s->timing_info.write_index += written;

    if (data == s->write_buffer) {
        free(s->write_buffer);
        s->write_buffer = NULL;
    } else {
        if (free_cb)
            free_cb((void *)data);
    }

    return 0;
}

APULSE_EXPORT
int
pa_stream_connect_record(pa_stream *s, const char *dev, const pa_buffer_attr *attr,
                         pa_stream_flags_t flags)
{
    gchar *s_attr = trace_pa_buffer_attr_as_string(attr);
    trace_info_f("P %s s=%p, dev=%s, attr=%s, flags=0x%x\n", __func__, s, dev, s_attr, flags);
    g_free(s_attr);

    s->direction = PA_STREAM_RECORD;
    stream_adjust_buffer_attrs(s, attr);

    if (do_connect_pcm(s, SND_PCM_STREAM_CAPTURE) != 0)
        goto err;

    snd_pcm_start(s->ph);

    return 0;
err:
    return -1;
}

APULSE_EXPORT
int
pa_stream_drop(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    ringbuffer_drop(s->rb, s->peek_buffer_data_len);
    return 0;
}

APULSE_EXPORT
const pa_buffer_attr *
pa_stream_get_buffer_attr(pa_stream *s)
{
    trace_info_f("F %s\n", __func__);

    return &s->buffer_attr;
}

APULSE_EXPORT
uint32_t
pa_stream_get_device_index(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    // apulse uses only one sink -- ALSA device, so index is always 0
    return 0;
}

APULSE_EXPORT
const char *
pa_stream_get_device_name(pa_stream *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);
    return "apulse";
}

APULSE_EXPORT
int
pa_stream_peek(pa_stream *s, const void **data, size_t *nbytes)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    if (!s)
        return -1;

    size_t len = ringbuffer_readable_size(s->rb);
    s->peek_buffer_data_len = ringbuffer_peek(s->rb, s->peek_buffer, len);

    if (nbytes)
        *nbytes = s->peek_buffer_data_len;
    if (data)
        *data = s->peek_buffer;
    return 0;
}

APULSE_EXPORT
void
pa_stream_set_read_callback(pa_stream *s, pa_stream_request_cb_t cb, void *userdata)
{
    trace_info_f("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    if (s) {
        s->read_cb = cb;
        s->read_cb_userdata = userdata;
    }
}

APULSE_EXPORT
void
pa_stream_set_underflow_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
}
