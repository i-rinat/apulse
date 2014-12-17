/*
 * Copyright © 2014  Rinat Ibragimov
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
    trace_info("P %s c=%p, server=%s, flags=%u, api=%p\n", __func__, c, server, flags, api);

    pa_context_ref(c);
    c->state = PA_CONTEXT_READY;
    c->mainloop_api->defer_new(c->mainloop_api, deh_context_state_changed, c);

    return 0;
}

APULSE_EXPORT
void
pa_context_disconnect(pa_context *c)
{
    trace_info("F %s c=%p\n", __func__, c);

    pa_context_ref(c);
    c->state = PA_CONTEXT_TERMINATED;
    c->mainloop_api->defer_new(c->mainloop_api, deh_context_state_changed, c);
}

APULSE_EXPORT
int
pa_context_errno(pa_context *c)
{
    trace_info("Z %s c=%p\n", __func__, c);

    return 0;
}

APULSE_EXPORT
pa_operation *
pa_context_get_sink_input_info(pa_context *c, uint32_t idx, pa_sink_input_info_cb_t cb,
                               void *userdata)
{
    trace_info("F %s c=%p, idx=%u, cb=%p, userdata=%p\n", __func__, c, idx, cb, userdata);

    pa_stream *s = g_hash_table_lookup(c->streams_ht, GINT_TO_POINTER(idx));
    if (!s) {
        trace_error("%s, no such stream: %d\n", __func__, idx);
        return NULL;
    }

    return pa_operation_new(c->mainloop_api, PAOP_CONTEXT_GET_SINK_INFO, c, s, cb, userdata);
}

APULSE_EXPORT
pa_context_state_t
pa_context_get_state(pa_context *c)
{
    trace_info("F %s c=%p\n", __func__, c);

    return c ? c->state : PA_CONTEXT_UNCONNECTED;
}

APULSE_EXPORT
pa_context *
pa_context_new(pa_mainloop_api *mainloop_api, const char *name)
{
    trace_info("F %s mainloop_api=%p, name=%s\n", __func__, mainloop_api, name);
    pa_proplist *p = pa_proplist_new();
    pa_context *c = pa_context_new_with_proplist(mainloop_api, name, p);
    pa_proplist_free(p);
    return c;
}

APULSE_EXPORT
pa_context *
pa_context_new_with_proplist(pa_mainloop_api *mainloop_api, const char *name, pa_proplist *proplist)
{
    trace_info("F %s mainloop_api=%p, name=%s, proplist=%p\n", __func__, mainloop_api, name,
               proplist);

    pa_context *c = calloc(1, sizeof(pa_context));
    c->state = PA_CONTEXT_UNCONNECTED;
    c->mainloop_api = mainloop_api;
    c->name = strdup(name ? name : "");
    c->ref_cnt = 1;
    c->streams_ht = g_hash_table_new(g_direct_hash, g_direct_equal);
    return c;
}

APULSE_EXPORT
pa_context *
pa_context_ref(pa_context *c)
{
    trace_info("F %s c=%p\n", __func__, c);

    if (!c)
        return NULL;
    c->ref_cnt ++;
    return c;
}

APULSE_EXPORT
pa_operation *
pa_context_set_sink_input_mute(pa_context *c, uint32_t idx, int mute, pa_context_success_cb_t cb,
                               void *userdata)
{
    trace_info("Z %s c=%p, idx=%u, mute=%d, cb=%p, userdata=%p\n", __func__, c, idx, mute, cb,
               userdata);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_set_sink_input_volume(pa_context *c, uint32_t idx, const pa_cvolume *volume,
                                 pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s c=%p, idx=%u, volume=%p, cb=%p, userdata=%p\n", __func__, c, idx, volume, cb,
               userdata);

    return NULL;
}

APULSE_EXPORT
void
pa_context_set_state_callback(pa_context *c, pa_context_notify_cb_t cb, void *userdata)
{
    trace_info("F %s c=%p, cb=%p, userdata=%p\n", __func__, c, cb, userdata);

    c->state_cb = cb;
    c->state_cb_userdata = userdata;
}

APULSE_EXPORT
void
pa_context_unref(pa_context *c)
{
    trace_info("F %s c=%p\n", __func__, c);

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
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_get_source_info_by_index(pa_context *c, uint32_t idx, pa_source_info_cb_t cb,
                                    void *userdata)
{
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_get_source_output_info(pa_context *c, uint32_t idx, pa_source_output_info_cb_t cb,
                                  void *userdata)
{
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
pa_operation *
pa_context_set_source_volume_by_index(pa_context *c, uint32_t idx, const pa_cvolume *volume,
                                      pa_context_success_cb_t cb, void *userdata)
{
    trace_info("Z %s c=%p, idx=%u, volume=%p, cb=%p, userdata=%p\n", __func__, c, idx, volume,
               cb, userdata);

    return NULL;
}

APULSE_EXPORT
void
pa_context_set_subscribe_callback(pa_context *c, pa_context_subscribe_cb_t cb, void *userdata)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
pa_operation *
pa_context_subscribe(pa_context *c, pa_subscription_mask_t m, pa_context_success_cb_t cb,
                     void *userdata)
{
    trace_info("Z %s\n", __func__);

    return NULL;
}
