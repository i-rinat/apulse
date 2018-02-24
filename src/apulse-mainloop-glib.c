/*
 * Copyright © 2017-2018  Rinat Ibragimov
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
#include <pulse/glib-mainloop.h>

static pa_io_event *
gml_api_io_new(pa_mainloop_api *a, int fd, pa_io_event_flags_t events,
               pa_io_event_cb_t cb, void *userdata)
{
    trace_info_z("Z %s a=%p, fd=%d, events=0x%x, cb=%p, userdata=%p\n",
                 __func__, a, fd, events, cb, userdata);
    return NULL;
}

static void
gml_api_io_enable(pa_io_event *e, pa_io_event_flags_t events)
{
    trace_info_z("Z %s e=%p, events=0x%x\n", __func__, e, events);
}

static void
gml_api_io_free(pa_io_event *e)
{
    trace_info_z("Z %s e=%p\n", __func__, e);
}

static void
gml_api_io_set_destroy(pa_io_event *e, pa_io_event_destroy_cb_t cb)
{
    trace_info_z("Z %s\n", __func__);
}

static pa_time_event *
gml_api_time_new(pa_mainloop_api *a, const struct timeval *tv,
                 pa_time_event_cb_t cb, void *userdata)
{
    trace_info_z("Z %s a=%p, tv=%p {%ld, %ld}, cb=%p, userdata=%p\n", __func__,
                 a, tv, tv ? tv->tv_sec : 0, tv ? tv->tv_usec : 0, cb,
                 userdata);
    return NULL;
}

static void
gml_api_time_restart(pa_time_event *e, const struct timeval *tv)
{
    trace_info_z("Z %s e=%p, tv=%p {%ld, %ld}\n", __func__, e, tv,
                 tv ? tv->tv_sec : 0, tv ? tv->tv_usec : 0);
}

static void
gml_api_time_free(pa_time_event *e)
{
    trace_info_z("Z %s e=%p\n", __func__, e);
}

static void
gml_api_time_set_destroy(pa_time_event *e, pa_time_event_destroy_cb_t cb)
{
    trace_info_f("Z %s e=%p, cb=%p\n", __func__, e, cb);
}

static pa_defer_event *
gml_api_defer_new(pa_mainloop_api *a, pa_defer_event_cb_t cb, void *userdata)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

static void
gml_api_defer_enable(pa_defer_event *e, int b)
{
    trace_info_z("Z %s\n", __func__);
}

static void
gml_api_defer_free(pa_defer_event *e)
{
    trace_info_z("Z %s\n", __func__);
}

static void
gml_api_defer_set_destroy(pa_defer_event *e, pa_defer_event_destroy_cb_t cb)
{
    trace_info_z("Z %s\n", __func__);
}

static void
gml_api_quit(pa_mainloop_api *a, int retval)
{
    trace_info_z("Z %s a=%p, retval=%d\n", __func__, a, retval);
}

APULSE_EXPORT
pa_glib_mainloop *
pa_glib_mainloop_new(GMainContext *c)
{
    trace_info_z("Z %s c=%p\n", __func__, c);

    pa_glib_mainloop *m = g_malloc0(sizeof(pa_glib_mainloop));

    m->api = (pa_mainloop_api){
        .userdata = m,
        .io_new = gml_api_io_new,
        .io_enable = gml_api_io_enable,
        .io_free = gml_api_io_free,
        .io_set_destroy = gml_api_io_set_destroy,
        .time_new = gml_api_time_new,
        .time_restart = gml_api_time_restart,
        .time_free = gml_api_time_free,
        .time_set_destroy = gml_api_time_set_destroy,
        .defer_new = gml_api_defer_new,
        .defer_enable = gml_api_defer_enable,
        .defer_free = gml_api_defer_free,
        .defer_set_destroy = gml_api_defer_set_destroy,
        .quit = gml_api_quit,
    };

    return m;
}

APULSE_EXPORT
void
pa_glib_mainloop_free(pa_glib_mainloop *g)
{
    trace_info_f("F %s g=%p\n", __func__, g);
    g_free(g);
}

APULSE_EXPORT
pa_mainloop_api *
pa_glib_mainloop_get_api(pa_glib_mainloop *g)
{
    trace_info_f("F %s g=%p\n", __func__, g);
    return &g->api;
}
