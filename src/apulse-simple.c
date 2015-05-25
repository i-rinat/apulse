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

#include <pulse/simple.h>
#include "apulse.h"
#include "trace.h"


APULSE_EXPORT
void
pa_simple_free(pa_simple *s)
{
    trace_info("Z %s s=%p\n", __func__, s);
}

APULSE_EXPORT
pa_simple *
pa_simple_new(const char *server, const char *name, pa_stream_direction_t dir, const char *dev,
              const char *stream_name, const pa_sample_spec *ss, const pa_channel_map *map,
              const pa_buffer_attr *attr, int *error)
{
    trace_info("Z %s server=%s, name=%s, dir=%d, dev=%s, stream_name=%s, ss=%p, map=%p, attr=%p\n",
               __func__, server, name, dir, dev, stream_name, ss, map, attr);
    return NULL;
}
