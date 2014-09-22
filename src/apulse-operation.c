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
deh_perform_operation(pa_mainloop_api *api, pa_defer_event *de, void *userdata)
{
    pa_operation *o = userdata;
    pa_stream *s;
    pa_context *c;
    pa_sink_input_info sii;

    switch (o->operation_type) {
    case PAOP_STREAM_SET_NAME:
        s = o->obj;
        free(s->name);
        s->name = o->param;
        if (o->cb)
            ((pa_stream_success_cb_t)o->cb)(s, 1, o->cb_userdata);
        break;
    case PAOP_STREAM_UPD_TIMING_INFO:
        s = o->obj;
        gettimeofday(&s->timing_info.timestamp, NULL);
        if (s->latency_update_cb)
            s->latency_update_cb(s, s->latency_update_cb_userdata);
        if (o->cb)
            ((pa_stream_success_cb_t)o->cb)(s, 1, o->cb_userdata);
        break;
    case PAOP_CONTEXT_GET_SINK_INFO:
        c = o->obj;
        s = o->param;

        sii.index = 0;
        sii.name = "dummy-sink";
        sii.owner_module = PA_INVALID_INDEX;
        sii.client = PA_INVALID_INDEX;
        sii.sink = 0;
        sii.sample_spec = (pa_sample_spec){.format = PA_SAMPLE_S16LE, .rate = 44100, .channels = 2};
        sii.channel_map.channels = 2;
        sii.channel_map.map[0] = PA_CHANNEL_POSITION_LEFT;
        sii.channel_map.map[1] = PA_CHANNEL_POSITION_RIGHT;
        sii.volume.channels = 2;
        sii.volume.values[0] = PA_VOLUME_NORM;
        sii.volume.values[1] = PA_VOLUME_NORM;
        sii.buffer_usec = 0;
        sii.sink_usec = 0;
        sii.resample_method = "auto";
        sii.driver = "guess who";
        sii.mute = 0;
        sii.proplist = pa_proplist_new();
        sii.corked = 0;
        sii.has_volume = 1;
        sii.volume_writable = 1;
        sii.format = NULL;

        if (o->cb)
            ((pa_sink_input_info_cb_t)o->cb)(c, &sii, 0, o->cb_userdata);

        pa_proplist_free(sii.proplist);
        break;
    case PAOP_STREAM_CORK:
    case PAOP_STREAM_FLUSH:
    case PAOP_STREAM_DRAIN:
    case PAOP_STREAM_TRIGGER:
        s = o->obj;
        if (o->cb)
            ((pa_stream_success_cb_t)o->cb)(s, 1, o->cb_userdata);
        break;
    default:
        break;
    }

    o->state = PA_OPERATION_DONE;
    pa_operation_unref(o);
}

pa_operation *
pa_operation_new(pa_mainloop_api *api, enum operation_type_e op_type, void *obj, void *param,
                 void *cb, void *cb_userdata)
{
    pa_operation *o = g_slice_new0(pa_operation);
    o->state = PA_OPERATION_RUNNING;
    o->ref_cnt = 1;
    o->operation_type = op_type;
    o->obj = obj;
    o->param = param;
    o->cb = cb;
    o->cb_userdata = cb_userdata;

    api->defer_new(api, deh_perform_operation, pa_operation_ref(o));
    return o;
}

APULSE_EXPORT
void
pa_operation_cancel(pa_operation *o)
{
    trace_info("Z %s o=%p\n", __func__, o);
}

APULSE_EXPORT
pa_operation_state_t
pa_operation_get_state(pa_operation *o)
{
    trace_info("F %s o=%p\n", __func__, o);
    return o->state;
}

APULSE_EXPORT
pa_operation *
pa_operation_ref(pa_operation *o)
{
    trace_info("F %s o=%p\n", __func__, o);

    if (!o)
        return NULL;
    o->ref_cnt ++;
    return o;
}

APULSE_EXPORT
void
pa_operation_unref(pa_operation *o)
{
    trace_info("F %s o=%p\n", __func__, o);

    if (!o)
        return;
    o->ref_cnt --;
    if (o->ref_cnt == 0)
        g_slice_free(pa_operation, o);
}
