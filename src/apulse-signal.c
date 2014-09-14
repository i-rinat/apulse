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


APULSE_EXPORT
int
pa_signal_init(pa_mainloop_api *api)
{
    trace_info("Z %s\n", __func__);

    return 0;
}

APULSE_EXPORT
void
pa_signal_done(void)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
pa_signal_event *
pa_signal_new(int sig, pa_signal_cb_t callback, void *userdata)
{
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
void
pa_signal_free(pa_signal_event *e)
{
    trace_info("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_signal_set_destroy(pa_signal_event *e, pa_signal_destroy_cb_t callback)
{
    trace_info("Z %s\n", __func__);
}
