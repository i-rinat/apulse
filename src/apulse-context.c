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


static
void
deh_context_state_changed(pa_mainloop_api *api, pa_defer_event *de, void *userdata)
{
    pa_context *c = userdata;
    if (c->state_cb)
        c->state_cb(c, c->state_cb_userdata);
    pa_context_unref(c);
}

APULSE_EXPORT
int
pa_context_connect(pa_context *c, const char *server, pa_context_flags_t flags,
                   const pa_spawn_api *api)
{
    trace_info_f("P %s c=%p, server=%s, flags=%u, api=%p\n", __func__, c, server, flags, api);

    pa_context_ref(c);
    c->state = PA_CONTEXT_READY;
    c->mainloop_api->defer_new(c->mainloop_api, deh_context_state_changed, c);

    return 0;
}

APULSE_EXPORT
void
pa_context_disconnect(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);

    pa_context_ref(c);
    c->state = PA_CONTEXT_TERMINATED;
    c->mainloop_api->defer_new(c->mainloop_api, deh_context_state_changed, c);
}

APULSE_EXPORT
int
pa_context_errno(pa_context *c)
{
    trace_info_z("Z %s c=%p\n", __func__, c);

    return 0;
}

APULSE_EXPORT
uint32_t
pa_context_get_protocol_version(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);
    return PA_PROTOCOL_VERSION;
}

static void
pa_context_get_server_info_impl(pa_operation *op)
{
    pa_server_info info = {
        .user_name = "apulse_user_name",    // TODO: actual user name
        .host_name = "apulse_host_name",    // TODO: actual host name
        .server_version = "5.0",
        .server_name = "pulseaudio",
        .sample_spec = {
            .format = PA_SAMPLE_S16LE,
            .rate = 44100,
            .channels = 2,
        },
        .default_sink_name = "default_sink_name",
        .default_source_name = "default_source_name",
        .cookie = 1,
        .channel_map = {
            .channels = 2,
            .map = {
                PA_CHANNEL_POSITION_FRONT_LEFT,
                PA_CHANNEL_POSITION_FRONT_RIGHT
            },
        },
    };

    if (op->server_info_cb)
        op->server_info_cb(op->c, &info, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_context_get_server_info(pa_context *c, pa_server_info_cb_t cb, void *userdata)
{
    trace_info_f("F %s c=%p, cb=%p, userdata=%p\n", __func__, c, &cb, userdata);

    pa_operation *op = pa_operation_new(c->mainloop_api, pa_context_get_server_info_impl);
    op->c = c;
    op->server_info_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
uint32_t
pa_context_get_server_protocol_version(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);
    return 8;   // PA headers say "8" is the protocol version used in PulseAudio 0.9
}

static void
pa_context_get_sink_info_by_name_impl(pa_operation *op)
{
    // TODO: real data
    pa_sink_info info = {
        .name = "default_sink_name",
        .index = 0,
        .description = "default and only sink",
        .sample_spec = {
            .format = PA_SAMPLE_S16LE,
            .rate = 44100,
            .channels = 2,
        },
        .channel_map = {
            .channels = 2,
            .map = {
                PA_CHANNEL_POSITION_FRONT_LEFT,
                PA_CHANNEL_POSITION_FRONT_RIGHT
            },
        },
        .owner_module = PA_INVALID_INDEX,
        .volume = {
            .channels = 2,
            .values = {
                PA_VOLUME_NORM,
                PA_VOLUME_NORM,
            },
        },
        .mute = 0,
        .monitor_source = 0,
        .monitor_source_name = "monitor_source_name",
        .latency = 100000,
        .driver = "apulse",
        .flags = 0,
        .proplist = NULL,
        .configured_latency = 100000,
        .base_volume = PA_VOLUME_NORM,
        .state = PA_SINK_RUNNING,
        .n_volume_steps = 0,
        .card = PA_INVALID_INDEX,
        .n_ports = 0,
        .ports = NULL,
        .active_port = NULL,
    };

    if (op->sink_info_cb) {
        op->sink_info_cb(op->c, &info, 0, op->cb_userdata);
        op->sink_info_cb(op->c, NULL, 1, op->cb_userdata);
    }

    free(op->char_ptr_arg_1);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_context_get_sink_info_by_name(pa_context *c, const char *name, pa_sink_info_cb_t cb,
                                 void *userdata)
{
    trace_info_f("F %s c=%p, name=%s, cb=%p, userdata=%p\n", __func__, c, name, cb, userdata);

    pa_operation *op = pa_operation_new(c->mainloop_api, pa_context_get_sink_info_by_name_impl);
    op->c = c;
    op->char_ptr_arg_1 = strdup(name ? name : "");
    op->sink_info_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

static void
pa_context_get_sink_input_info_impl(pa_operation *op)
{
    uint32_t idx = op->int_arg_1;
    pa_stream *s = g_hash_table_lookup(op->c->streams_ht, GINT_TO_POINTER(idx));
    if (!s) {
        trace_error("%s, no such stream: %u\n", __func__, idx);
        pa_operation_cancel(op);
        return;
    }

    pa_sink_input_info sii = {
        .index = 0,
        .name = "dummy-sink",
        .owner_module = PA_INVALID_INDEX,
        .client = PA_INVALID_INDEX,
        .sink = 0,
        .sample_spec = (pa_sample_spec){.format = PA_SAMPLE_S16LE, .rate = 44100, .channels = 2},
        .channel_map.channels = 2,
        .channel_map.map[0] = PA_CHANNEL_POSITION_LEFT,
        .channel_map.map[1] = PA_CHANNEL_POSITION_RIGHT,
        .volume.channels = 2,
        .volume.values[0] = PA_VOLUME_NORM,
        .volume.values[1] = PA_VOLUME_NORM,
        .buffer_usec = 0,
        .sink_usec = 0,
        .resample_method = "auto",
        .driver = "guess who",
        .mute = 0,
        .proplist = pa_proplist_new(),
        .corked = 0,
        .has_volume = 1,
        .volume_writable = 1,
        .format = NULL,
    };

    if (op->sink_input_info_cb)
        op->sink_input_info_cb(op->c, &sii, 0, op->cb_userdata);

    pa_proplist_free(sii.proplist);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_context_get_sink_input_info(pa_context *c, uint32_t idx, pa_sink_input_info_cb_t cb,
                               void *userdata)
{
    trace_info_f("F %s c=%p, idx=%u, cb=%p, userdata=%p\n", __func__, c, idx, cb, userdata);

    pa_operation *op = pa_operation_new(c->mainloop_api, pa_context_get_sink_input_info_impl);
    op->c = c;
    op->int_arg_1 = idx;
    op->sink_input_info_cb = cb;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
pa_context_state_t
pa_context_get_state(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);

    return c ? c->state : PA_CONTEXT_UNCONNECTED;
}

APULSE_EXPORT
pa_context *
pa_context_new(pa_mainloop_api *mainloop_api, const char *name)
{
    trace_info_f("F %s mainloop_api=%p, name=%s\n", __func__, mainloop_api, name);
    pa_proplist *p = pa_proplist_new();
    pa_context *c = pa_context_new_with_proplist(mainloop_api, name, p);
    pa_proplist_free(p);
    return c;
}

APULSE_EXPORT
pa_context *
pa_context_new_with_proplist(pa_mainloop_api *mainloop_api, const char *name, pa_proplist *proplist)
{
    trace_info_f("F %s mainloop_api=%p, name=%s, proplist=%p\n", __func__, mainloop_api, name,
               proplist);

    pa_context *c = calloc(1, sizeof(pa_context));
    c->state = PA_CONTEXT_UNCONNECTED;
    c->mainloop_api = mainloop_api;
    c->name = strdup(name ? name : "");
    c->ref_cnt = 1;
    c->streams_ht = g_hash_table_new(g_direct_hash, g_direct_equal);

    for (uint32_t k = 0; k < PA_CHANNELS_MAX; k++) {
        c->sink_volume[k] = PA_VOLUME_NORM;
        c->source_volume[k] = PA_VOLUME_NORM;
    }

    return c;
}

APULSE_EXPORT
pa_context *
pa_context_ref(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);

    if (!c)
        return NULL;
    c->ref_cnt ++;
    return c;
}

APULSE_EXPORT
pa_time_event *
pa_context_rttime_new(pa_context *c, pa_usec_t usec, pa_time_event_cb_t cb, void *userdata)
{
    trace_info_z("Z %s c=%p, usec=%" PRIu64 " cb=%p, userdata=%p\n", __func__, c, (uint64_t)usec,
                 cb, userdata);
    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_set_sink_input_mute(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb,
                               void *userdata)
{
    trace_info_z("Z %s c=%p, idx=%u, mute=%d, cb=%p, userdata=%p\n", __func__, c, idx, mute, cb,
               userdata);

    return NULL;
}

static void
pa_context_set_sink_input_volume_impl(pa_operation *op)
{
    memset(&op->c->sink_volume, 0, sizeof(op->c->sink_volume));

    const uint32_t channels = MIN(op->pa_cvolume_arg_1.channels, PA_CHANNELS_MAX);

    for (uint32_t k = 0; k < channels; k++)
        op->c->sink_volume[k] = op->pa_cvolume_arg_1.values[k];

    if (op->context_success_cb)
        op->context_success_cb(op->c, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_context_set_sink_input_volume(pa_context *c, uint32_t idx, const pa_cvolume *volume,
                                 pa_context_success_cb_t cb, void *userdata)
{
    gchar *s_volume = trace_pa_volume_as_string(volume);
    trace_info_f("F %s c=%p, idx=%u, volume=%s, cb=%p, userdata=%p\n", __func__, c, idx, s_volume,
                 cb, userdata);
    g_free(s_volume);

    pa_operation *op = pa_operation_new(c->mainloop_api, pa_context_set_sink_input_volume_impl);
    op->c = c;
    op->int_arg_1 = idx;
    op->context_success_cb = cb;
    op->cb_userdata = userdata;

    if (volume)
        op->pa_cvolume_arg_1 = *volume;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
void
pa_context_set_state_callback(pa_context *c, pa_context_notify_cb_t cb, void *userdata)
{
    trace_info_f("F %s c=%p, cb=%p, userdata=%p\n", __func__, c, cb, userdata);

    c->state_cb = cb;
    c->state_cb_userdata = userdata;
}

APULSE_EXPORT
void
pa_context_unref(pa_context *c)
{
    trace_info_f("F %s c=%p\n", __func__, c);

    if (!c)
        return;
    c->ref_cnt --;
    if (c->ref_cnt == 0) {
        g_hash_table_unref(c->streams_ht);
        free(c->name);
        free(c);
    }
}

APULSE_EXPORT
pa_operation *
pa_context_drain(pa_context *c, pa_context_notify_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_get_source_info_by_index(pa_context *c, uint32_t idx, pa_source_info_cb_t cb,
                                    void *userdata)
{
    trace_info_z("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_get_source_output_info(pa_context *c, uint32_t idx, pa_source_output_info_cb_t cb,
                                  void *userdata)
{
    trace_info_z("Z %s\n", __func__);

    return NULL;
}

static void
pa_context_set_source_volume_by_index_impl(pa_operation *op)
{
    // TODO: actually change volume

    if (op->context_success_cb)
        op->context_success_cb(op->c, 1, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
pa_operation *
pa_context_set_source_volume_by_index(pa_context *c, uint32_t idx, const pa_cvolume *volume,
                                      pa_context_success_cb_t cb, void *userdata)
{
    gchar *s_volume = trace_pa_volume_as_string(volume);
    trace_info_f("Z %s c=%p, idx=%u, volume=%s, cb=%p, userdata=%p\n", __func__, c, idx, s_volume,
                 cb, userdata);
    g_free(s_volume);

    pa_operation *op =
        pa_operation_new(c->mainloop_api, pa_context_set_source_volume_by_index_impl);
    op->c = c;
    op->context_success_cb = cb;
    op->cb_userdata = userdata;

    if (volume)
        op->pa_cvolume_arg_1 = *volume;

    pa_operation_launch(op);
    return op;
}

APULSE_EXPORT
void
pa_context_set_subscribe_callback(pa_context *c, pa_context_subscribe_cb_t cb, void *userdata)
{
    trace_info_z("Z %s c=%p, cb=%p, userdata=%p\n", __func__, c, cb, userdata);
}

APULSE_EXPORT
pa_operation *
pa_context_subscribe(pa_context *c, pa_subscription_mask_t m, pa_context_success_cb_t cb,
                     void *userdata)
{
    trace_info_z("Z %s c=%p, m=0x%04x, cb=%p, userdata=%p\n", __func__, c, m, cb, userdata);

    return NULL;
}
