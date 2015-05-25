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


#define CHECK_A(funcname, params)                                           \
    do {                                                                    \
        int errcode___ = funcname params;                                   \
        if (errcode___ < 0) {                                               \
            trace_error("%s, " #funcname ", %s\n", __func__,                \
                        snd_strerror(errcode___));                          \
            goto err;                                                       \
        }                                                                   \
    } while (0)


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
    int                 paused = g_atomic_int_get(&s->paused);

    if (events & (PA_IO_EVENT_INPUT | PA_IO_EVENT_OUTPUT)) {
        frame_count = snd_pcm_avail(s->ph);
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
            frame_count = snd_pcm_avail(s->ph);
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
            size_t bytecnt = MIN(sizeof(buf), frame_count * frame_size);
            memset(buf, 0, bytecnt);
            snd_pcm_writei(s->ph, buf, bytecnt / frame_size);
        } else {
            size_t writable_size = pa_stream_writable_size(s);

            if (s->write_cb && writable_size > 0)
                s->write_cb(s, writable_size, s->write_cb_userdata);

            size_t bytecnt = MIN(sizeof(buf), frame_count * frame_size);
            bytecnt = ringbuffer_read(s->rb, buf, bytecnt);

            if (bytecnt == 0) {
                // application is not ready yet, play silence
                bytecnt = MIN(sizeof(buf), frame_count * frame_size);
                memset(buf, 0, bytecnt);
            }
            snd_pcm_writei(s->ph, buf, bytecnt / frame_size);
        }
    }

    if (events & PA_IO_EVENT_INPUT) {
        if (paused) {
            // client stream is corked. Read data from ALSA and discard them
            size_t bytecnt = MIN(sizeof(buf), frame_count * frame_size);
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
            bytecnt = MIN(bytecnt, sizeof(buf));

            if (bytecnt > 0) {
                snd_pcm_readi(s->ph, buf, bytecnt / frame_size);
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
    int dir;
    unsigned int rate;
    const char *dev_name;

    switch (stream_direction) {
    default:
    case SND_PCM_STREAM_PLAYBACK:
        dev_name = getenv("APULSE_PLAYBACK_DEVICE");
        CHECK_A(snd_pcm_open, (&s->ph, dev_name ? dev_name : "default", stream_direction, 0));
        break;
    case SND_PCM_STREAM_CAPTURE:
        dev_name = getenv("APULSE_CAPTURE_DEVICE");
        CHECK_A(snd_pcm_open, (&s->ph, dev_name ? dev_name : "default", stream_direction, 0));
        break;
    }

    CHECK_A(snd_pcm_hw_params_malloc, (&hw_params));
    CHECK_A(snd_pcm_hw_params_any, (s->ph, hw_params));
    CHECK_A(snd_pcm_hw_params_set_access, (s->ph, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED));
    CHECK_A(snd_pcm_hw_params_set_format, (s->ph, hw_params, pa_format_to_alsa(s->ss.format)));
    CHECK_A(snd_pcm_hw_params_set_rate_resample, (s->ph, hw_params, 1));
    rate = s->ss.rate;
    dir = 0;
    CHECK_A(snd_pcm_hw_params_set_rate_near, (s->ph, hw_params, &rate, &dir));
    CHECK_A(snd_pcm_hw_params_set_channels, (s->ph, hw_params, s->ss.channels));

    unsigned int period_time = 20 * 1000;
    dir = 1;
    CHECK_A(snd_pcm_hw_params_set_period_time_near, (s->ph, hw_params, &period_time, &dir));
    dir = -1;
    snd_pcm_uframes_t period_size;
    CHECK_A(snd_pcm_hw_params_get_period_size, (hw_params, &period_size, &dir));

    unsigned int buffer_time = 4 * period_time;
    dir = 1;
    CHECK_A(snd_pcm_hw_params_set_buffer_time_near, (s->ph, hw_params, &buffer_time, &dir));
    CHECK_A(snd_pcm_hw_params, (s->ph, hw_params));
    snd_pcm_hw_params_free(hw_params);

    CHECK_A(snd_pcm_sw_params_malloc, (&sw_params));
    CHECK_A(snd_pcm_sw_params_current, (s->ph, sw_params));
    CHECK_A(snd_pcm_sw_params_set_avail_min, (s->ph, sw_params, period_size));
    // no period event requested
    CHECK_A(snd_pcm_sw_params, (s->ph, sw_params));
    snd_pcm_sw_params_free(sw_params);

    CHECK_A(snd_pcm_prepare, (s->ph));

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

    return 0;
err:
    return -1;
}

APULSE_EXPORT
int
pa_stream_connect_playback(pa_stream *s, const char *dev, const pa_buffer_attr *attr,
                           pa_stream_flags_t flags, const pa_cvolume *volume,
                           pa_stream *sync_stream)
{
    gchar *s_attr = trace_pa_buffer_attr_as_string(attr);
    trace_info("P %s s=%p, dev=%s, attr=%s, flags=0x%x, volume=%p, sync_stream=%p\n", __func__,
               s, dev, s_attr, flags, volume, sync_stream);
    g_free(s_attr);

    s->direction = PA_STREAM_PLAYBACK;
    if (attr) {
        s->buffer_attr = *attr;
    } else {
        s->buffer_attr.maxlength = (uint32_t)-1;
        s->buffer_attr.tlength = (uint32_t)-1;
        s->buffer_attr.prebuf = (uint32_t)-1;
        s->buffer_attr.minreq = (uint32_t)-1;
        s->buffer_attr.fragsize = (uint32_t)-1;
    }

    if (do_connect_pcm(s, SND_PCM_STREAM_PLAYBACK) != 0)
        goto err;

    g_atomic_int_set(&s->paused, !!(flags & PA_STREAM_START_CORKED));

    return 0;
err:
    return -1;
}

APULSE_EXPORT
pa_operation *
pa_stream_cork(pa_stream *s, int b, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, b=%d, cb=%p, userdata=%p\n", __func__, s, b, cb, userdata);

    g_atomic_int_set(&s->paused, !!(b));
    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_CORK, s, GINT_TO_POINTER(b),
                            cb, userdata);
}

APULSE_EXPORT
int
pa_stream_disconnect(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    for (int k = 0; k < s->nioe; k ++) {
        pa_mainloop_api *api = s->c->mainloop_api;
        api->io_free(s->ioe[k]);
    }
    free(s->ioe);

    if (snd_pcm_close(s->ph) == 0)
        return 0;

    return -1;
}

APULSE_EXPORT
pa_operation *
pa_stream_drain(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    snd_pcm_drain(s->ph);
    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_DRAIN, s, NULL, cb, userdata);
}

APULSE_EXPORT
pa_operation *
pa_stream_flush(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    // TODO: is it ok to do nothing?
    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_FLUSH, s, NULL, cb, userdata);
}

APULSE_EXPORT
uint32_t
pa_stream_get_index(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    return s->idx;
}

APULSE_EXPORT
int
pa_stream_get_latency(pa_stream *s, pa_usec_t *r_usec, int *negative)
{
    trace_info("F %s s=%p\n", __func__, s);

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
    trace_info("F %s s=%p\n", __func__, s);

    return &s->ss;
}

APULSE_EXPORT
pa_stream_state_t
pa_stream_get_state(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    return s->state;
}

APULSE_EXPORT
const pa_timing_info *
pa_stream_get_timing_info(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

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
    trace_info("F %s s=%p\n", __func__, s);
    return g_atomic_int_get(&s->paused);
}

APULSE_EXPORT
int
pa_stream_is_suspended(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);
    // ALSA sink is never suspended
    return 0;
}

APULSE_EXPORT
pa_stream *
pa_stream_new(pa_context *c, const char *name, const pa_sample_spec *ss, const pa_channel_map *map)
{
    trace_info("F %s c=%p, name=%s, ss=%p, map=%p\n", __func__, c, name, ss, map);

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
    trace_info("P %s c=%p, name=%s, formats=%p, n_formats=%u, p=%p\n", __func__, c, name,
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
    trace_info("F %s c=%p, name=%s, ss={.format=%d, .rate=%u, .channels=%u}, map=%p, p=%p\n",
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

    s->rb = ringbuffer_new(50 * 1024);    // TODO: figure out size
    s->peek_buffer = malloc(s->rb->end - s->rb->start);

    return s;
}

APULSE_EXPORT
pa_stream *
pa_stream_ref(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    s->ref_cnt ++;
    return s;
}

APULSE_EXPORT
void
pa_stream_set_latency_update_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->latency_update_cb = cb;
    s->latency_update_cb_userdata = userdata;
}

APULSE_EXPORT
pa_operation *
pa_stream_set_name(pa_stream *s, const char *name, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("P %s s=%p, name=%s, cb=%p, userdata=%p\n", __func__, s, name, cb, userdata);

    char *new_name = strdup(name ? name : "");
    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_SET_NAME, s, new_name, cb, userdata);
}

APULSE_EXPORT
void
pa_stream_set_state_callback(pa_stream *s, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->state_cb = cb;
    s->state_cb_userdata = userdata;
}

APULSE_EXPORT
void
pa_stream_set_write_callback(pa_stream *s, pa_stream_request_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    s->write_cb = cb;
    s->write_cb_userdata = userdata;
}

APULSE_EXPORT
pa_operation *
pa_stream_trigger(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_TRIGGER, s, NULL, cb, userdata);
}

APULSE_EXPORT
void
pa_stream_unref(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    s->ref_cnt --;
    if (s->ref_cnt == 0) {
        g_hash_table_remove(s->c->streams_ht, GINT_TO_POINTER(s->idx));
        ringbuffer_free(s->rb);
        free(s->peek_buffer);
        free(s->name);
        free(s);
    }
}

APULSE_EXPORT
pa_operation *
pa_stream_update_timing_info(pa_stream *s, pa_stream_success_cb_t cb, void *userdata)
{
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    return pa_operation_new(s->c->mainloop_api, PAOP_STREAM_UPD_TIMING_INFO, s, NULL, cb, userdata);
}

APULSE_EXPORT
size_t
pa_stream_writable_size(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    return ringbuffer_writable_size(s->rb);
}

APULSE_EXPORT
size_t pa_stream_readable_size(pa_stream *s)
{
    trace_info("F %s s=%p\n", __func__, s);

    return ringbuffer_readable_size(s->rb);
}

APULSE_EXPORT
int
pa_stream_write(pa_stream *s, const void *data, size_t nbytes, pa_free_cb_t free_cb, int64_t offset,
                pa_seek_mode_t seek)
{
    trace_info("F %s s=%p, data=%p, nbytes=%zu, free_cb=%p, offset=%"PRId64", seek=%u\n", __func__,
               s, data, nbytes, free_cb, offset, seek);

    if (offset != 0)
        trace_error("%s, offset != 0\n", __func__);
    if (seek != PA_SEEK_RELATIVE)
        trace_error("%s, seek != PA_SEEK_RELATIVE\n", __func__);

    size_t written = ringbuffer_write(s->rb, data, nbytes);
    s->timing_info.since_underrun += written;
    s->timing_info.write_index += written;
    if (free_cb)
        free_cb((void *)data);

    return 0;
}

APULSE_EXPORT
int
pa_stream_connect_record(pa_stream *s, const char *dev, const pa_buffer_attr *attr,
                         pa_stream_flags_t flags)
{
    gchar *s_attr = trace_pa_buffer_attr_as_string(attr);
    trace_info("P %s s=%p, dev=%s, attr=%s, flags=0x%x\n", __func__, s, dev, s_attr, flags);
    g_free(s_attr);

    s->direction = PA_STREAM_RECORD;
    if (attr) {
        s->buffer_attr = *attr;
    } else {
        s->buffer_attr.maxlength = (uint32_t)-1;
        s->buffer_attr.tlength = (uint32_t)-1;
        s->buffer_attr.prebuf = (uint32_t)-1;
        s->buffer_attr.minreq = (uint32_t)-1;
        s->buffer_attr.fragsize = (uint32_t)-1;
    }

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
    trace_info("F %s s=%p\n", __func__, s);

    ringbuffer_drop(s->rb, s->peek_buffer_data_len);
    return 0;
}

APULSE_EXPORT
const pa_buffer_attr *
pa_stream_get_buffer_attr(pa_stream *s)
{
    trace_info("F %s\n", __func__);

    return &s->buffer_attr;
}

APULSE_EXPORT
uint32_t
pa_stream_get_device_index(pa_stream *s)
{
    trace_info("Z %s s=%p\n", __func__, s);

    return 0;
}

APULSE_EXPORT
int
pa_stream_peek(pa_stream *s, const void **data, size_t *nbytes)
{
    trace_info("F %s s=%p\n", __func__, s);

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
    trace_info("F %s s=%p, cb=%p, userdata=%p\n", __func__, s, cb, userdata);

    if (s) {
        s->read_cb = cb;
        s->read_cb_userdata = userdata;
    }
}

APULSE_EXPORT
void
pa_stream_set_underflow_callback(pa_stream *p, pa_stream_notify_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}
