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
deh_perform_operation(pa_mainloop_api *api, pa_defer_event *de, void *userdata)
{
    pa_operation *op = userdata;
    op->handler(op);
}

pa_operation *
pa_operation_new(pa_mainloop_api *api, void (*handler)(pa_operation *op))
{
    pa_operation *o = g_slice_new0(pa_operation);
    o->state = PA_OPERATION_RUNNING;
    o->ref_cnt = 1;
    o->api = api;
    o->handler = handler;

    return o;
}

void
pa_operation_launch(pa_operation *op)
{
    pa_mainloop_api *api = op->api;
    api->defer_new(api, deh_perform_operation, pa_operation_ref(op));
}

void
pa_operation_done(pa_operation *op)
{
    op->state = PA_OPERATION_DONE;
    pa_operation_unref(op);
}

APULSE_EXPORT
void
pa_operation_cancel(pa_operation *o)
{
    trace_info_z("Z %s o=%p\n", __func__, o);
}

APULSE_EXPORT
pa_operation_state_t
pa_operation_get_state(pa_operation *o)
{
    trace_info_f("F %s o=%p\n", __func__, o);
    if (!o) {
        trace_error("Z %s operation is NULL\n", __func__);
        return PA_OPERATION_DONE;
    }
    return o->state;
}

APULSE_EXPORT
pa_operation *
pa_operation_ref(pa_operation *o)
{
    trace_info_f("F %s o=%p\n", __func__, o);

    if (!o)
        return NULL;
    o->ref_cnt ++;
    return o;
}

APULSE_EXPORT
void
pa_operation_unref(pa_operation *o)
{
    trace_info_f("F %s o=%p\n", __func__, o);

    if (!o)
        return;
    o->ref_cnt --;
    if (o->ref_cnt == 0)
        g_slice_free(pa_operation, o);
}
