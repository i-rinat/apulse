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
pa_channel_map *
pa_channel_map_init_auto(pa_channel_map *m, unsigned channels, pa_channel_map_def_t def)
{
    trace_info("Z %s m=%p, channels=%u, def=%u\n", __func__, m, channels, def);

    return NULL;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_mono(pa_channel_map *m)
{
    trace_info("F %s\n", __func__);

    pa_channel_map *cm = calloc(1, sizeof(pa_channel_map));
    cm->channels = 1;
    cm->map[0] = PA_CHANNEL_POSITION_MONO;

    return cm;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_stereo(pa_channel_map *m)
{
    trace_info("F %s\n", __func__);

    pa_channel_map *cm = calloc(1, sizeof(pa_channel_map));
    cm->channels = 2;
    cm->map[0] = PA_CHANNEL_POSITION_FRONT_LEFT;
    cm->map[1] = PA_CHANNEL_POSITION_FRONT_RIGHT;

    return cm;
}

APULSE_EXPORT
pa_channel_map *
pa_channel_map_init_extend(pa_channel_map *m, unsigned channels, pa_channel_map_def_t def)
{
    trace_info("Z %s\n", __func__);

    return NULL;
}

APULSE_EXPORT
int
pa_channel_map_compatible(const pa_channel_map *map, const pa_sample_spec *ss)
{
    trace_info("Z %s\n", __func__);

    return 1;
}
