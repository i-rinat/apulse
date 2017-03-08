/*
 * Copyright © 2014-2017  Rinat Ibragimov
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
#include <assert.h>
#include <math.h>

APULSE_EXPORT
pa_volume_t
pa_sw_volume_from_linear(double v)
{
    trace_info_f("F %s v=%f\n", __func__, v);

    if (v <= 0.0)
        return PA_VOLUME_MUTED;

    int64_t v_linear = lround(PA_VOLUME_NORM * cbrt(v));
    assert(v_linear >= 0);

    return MIN(v_linear, PA_VOLUME_MAX);
}

APULSE_EXPORT
double
pa_sw_volume_to_linear(pa_volume_t v)
{
    trace_info_f("F %s v=%u\n", __func__, v);

    uint32_t v_clamped = MIN(v, PA_VOLUME_MAX);

    double v_linear = v_clamped * (1.0 / PA_VOLUME_NORM);
    return v_linear * v_linear * v_linear;
}
