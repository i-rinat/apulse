/*
 * Copyright © 2014-2018  Rinat Ibragimov
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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
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
#include <pulse/simple.h>

APULSE_EXPORT
void
pa_simple_free(pa_simple *s)
{
    trace_info_f("F %s s=%p\n", __func__, s);

    if (s->initialized) {
        pa_stream_unref(s->stream);
        pa_context_disconnect(s->context);
        pa_context_unref(s->context);
        pa_threaded_mainloop_free(s->mainloop);

        s->initialized = 0;
    }

    free(s);
}

static void
pai_simple_context_state_cb(pa_context *c, void *user_data)
{
    pa_threaded_mainloop *mainloop = user_data;

    switch (pa_context_get_state(c)) {
    case PA_CONTEXT_READY:
    case PA_CONTEXT_TERMINATED:
    case PA_CONTEXT_FAILED:
        pa_threaded_mainloop_signal(mainloop, 0);
        break;
    default:
        break;
    }
}

static void
pai_simple_stream_state_cb(pa_stream *s, void *user_data)
{
    pa_simple *simple = user_data;

    switch (pa_stream_get_state(s)) {
    case PA_STREAM_READY:
    case PA_STREAM_FAILED:
    case PA_STREAM_TERMINATED:
        pa_threaded_mainloop_signal(simple->mainloop, 0);
        break;
    default:
        break;
    }
}

static void
pai_simple_stream_write_cb(pa_stream *s, size_t length, void *user_data)
{
    pa_simple *simple = user_data;
    pa_threaded_mainloop_signal(simple->mainloop, 0);
}

static void
pai_simple_stream_read_cb(pa_stream *s, size_t length, void *user_data)
{
    pa_simple *simple = user_data;
    pa_threaded_mainloop_signal(simple->mainloop, 0);
}

static void
pai_simple_stream_latency_update_cb(pa_stream *s, void *user_data)
{
    pa_simple *simple = user_data;
    pa_threaded_mainloop_signal(simple->mainloop, 0);
}

static int
pai_simple_stream_connect(pa_simple *simple, pa_stream_direction_t dir,
                          const char *stream_name, const pa_sample_spec *ss,
                          const pa_buffer_attr *attr)
{
    simple->stream = pa_stream_new(simple->context, stream_name, ss, NULL);
    if (!simple->stream) {
        trace_error("%s: can't create stream", __func__);
        goto err_1;
    }

    pa_stream_set_state_callback(simple->stream, pai_simple_stream_state_cb,
                                 simple);
    pa_stream_set_read_callback(simple->stream, pai_simple_stream_read_cb,
                                simple);
    pa_stream_set_write_callback(simple->stream, pai_simple_stream_write_cb,
                                 simple);
    pa_stream_set_latency_update_callback(
        simple->stream, pai_simple_stream_latency_update_cb, simple);

    if (dir == PA_STREAM_PLAYBACK) {
        if (pa_stream_connect_playback(simple->stream, NULL, attr, 0, NULL,
                                       NULL) < 0)
        {
            trace_error("%s: can't connect playback stream", __func__);
            goto err_2;
        }
    } else {
        int flags = PA_STREAM_ADJUST_LATENCY;
        if (pa_stream_connect_record(simple->stream, NULL, attr, flags) < 0) {
            trace_error("%s: can't connect record stream", __func__);
            goto err_2;
        }
    }

    while (1) {
        pa_stream_state_t state = pa_stream_get_state(simple->stream);
        if (state == PA_STREAM_READY)
            break;
        if (!PA_STREAM_IS_GOOD(state)) {
            trace_error("%s, stream is not ready\n", __func__);
            goto err_2;
        }
        pa_threaded_mainloop_wait(simple->mainloop);
    }

    return 0;

err_2:
    pa_stream_unref(simple->stream);

err_1:
    return -1;
}

APULSE_EXPORT
pa_simple *
pa_simple_new(const char *server, const char *name, pa_stream_direction_t dir,
              const char *dev, const char *stream_name,
              const pa_sample_spec *ss, const pa_channel_map *map,
              const pa_buffer_attr *attr, int *error)
{
    gchar *s_map = trace_pa_channel_map_as_string(map);
    gchar *s_ss = trace_pa_sample_spec_as_string(ss);
    trace_info_f(
        "F %s server=%s, name=%s, dir=%d, dev=%s, stream_name=%s, ss=%s, "
        "map=%s, attr=%p\n",
        __func__, server, name, dir, dev, stream_name, s_ss, s_map, attr);
    g_free(s_ss);
    g_free(s_map);

    pa_simple *simple = calloc(sizeof(*simple), 1);
    if (!simple) {
        trace_error("%s: can't allocate memory", __func__);
        return NULL;
    }

    simple->mainloop = pa_threaded_mainloop_new();
    if (!simple->mainloop) {
        trace_error("%s: can't create mainloop", __func__);
        goto err_1;
    }

    simple->context =
        pa_context_new(pa_threaded_mainloop_get_api(simple->mainloop), name);
    if (!simple->context) {
        trace_error("%s: can't create context", __func__);
        goto err_2;
    }

    pa_context_set_state_callback(simple->context, pai_simple_context_state_cb,
                                  simple->mainloop);

    if (pa_context_connect(simple->context, NULL, PA_CONTEXT_NOFLAGS, NULL) < 0)
    {
        trace_error("%s: can't connect context", __func__);
        goto err_3;
    }

    pa_threaded_mainloop_lock(simple->mainloop);

    if (pa_threaded_mainloop_start(simple->mainloop) < 0) {
        trace_error("%s: can't start mainloop", __func__);
        goto err_4;
    }

    pa_threaded_mainloop_wait(simple->mainloop);

    if (pa_context_get_state(simple->context) != PA_CONTEXT_READY) {
        trace_error("%s: can't get context ready", __func__);
        goto err_5;
    }

    if (pai_simple_stream_connect(simple, dir, stream_name, ss, attr) < 0) {
        trace_error("%s: can't connect stream", __func__);
        goto err_5;
    }

    pa_threaded_mainloop_unlock(simple->mainloop);

    simple->initialized = 1;
    return simple;

err_5:
    pa_threaded_mainloop_unlock(simple->mainloop);

err_4:
    pa_context_disconnect(simple->context);

err_3:
    pa_context_unref(simple->context);

err_2:
    pa_threaded_mainloop_free(simple->mainloop);

err_1:
    free(simple);

    // TODO: error codes
    if (error)
        *error = -1;

    return NULL;
}

APULSE_EXPORT
int
pa_simple_write(pa_simple *s, const void *data, size_t bytes, int *error)
{
    trace_info_f("F %s s=%p, data=%p, bytes=%zu, error=%p\n", __func__, s, data,
                 bytes, error);

    size_t to_write = bytes;
    const char *p = data;

    pa_threaded_mainloop_lock(s->mainloop);

    while (to_write > 0) {
        size_t chunk_size = pa_stream_writable_size(s->stream);
        while (chunk_size == 0) {
            // Sleep here until write callback is called.
            pa_threaded_mainloop_wait(s->mainloop);
            chunk_size = pa_stream_writable_size(s->stream);
        }

        chunk_size = MIN(chunk_size, to_write);
        if (pa_stream_write(s->stream, p, chunk_size, NULL, 0,
                            PA_SEEK_RELATIVE) < 0)
        {
            trace_error("%s: can't write", __func__);
            goto err;
        }

        p += chunk_size;
        to_write -= chunk_size;
    }

    pa_threaded_mainloop_unlock(s->mainloop);

    return 0;

err:
    pa_threaded_mainloop_unlock(s->mainloop);
    return -1;
}

APULSE_EXPORT
int
pa_simple_drain(pa_simple *s, int *error)
{
    trace_info_z("Z %s\n", __func__);
    return -1;
}

APULSE_EXPORT
int
pa_simple_read(pa_simple *s, void *data, size_t bytes, int *error)
{
    trace_info_z("Z %s\n", __func__);
    return -1;
}

APULSE_EXPORT
pa_usec_t
pa_simple_get_latency(pa_simple *s, int *error)
{
    trace_info_f("F %s s=%p, error=%p\n", __func__, s, error);

    pa_usec_t latency = 0;
    int negative;
    pa_stream_get_latency(s->stream, &latency, &negative);

    return latency;
}

APULSE_EXPORT
int
pa_simple_flush(pa_simple *s, int *error)
{
    trace_info_z("Z %s\n", __func__);
    return -1;
}
