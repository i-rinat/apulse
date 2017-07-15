/*
 * Copyright © 2017  Rinat Ibragimov
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
#include <pulse/glib-mainloop.h>

APULSE_EXPORT
pa_glib_mainloop *
pa_glib_mainloop_new(GMainContext *c)
{
    trace_info_z("Z %s c=%p\n", __func__, c);
    return NULL;
}

APULSE_EXPORT
void
pa_glib_mainloop_free(pa_glib_mainloop *g)
{
    trace_info_z("Z %s g=%p\n", __func__, g);
}

APULSE_EXPORT
pa_mainloop_api *
pa_glib_mainloop_get_api(pa_glib_mainloop *g)
{
    trace_info_z("Z %s g=%p\n", __func__, g);
    return NULL;
}
