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

APULSE_EXPORT
char *
pa_cvolume_snprint_verbose(char *s, size_t l, const pa_cvolume *c, const pa_channel_map *map,
                           int print_dB)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_volume_snprint(char *s, size_t l, pa_volume_t v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_sw_volume_snprint_dB(char *s, size_t l, pa_volume_t v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_volume_snprint_verbose(char *s, size_t l, pa_volume_t v, int print_dB)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_avg_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_max(const pa_cvolume *a)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_max_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_min(const pa_cvolume *a)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_min_mask(const pa_cvolume *a, const pa_channel_map *cm, pa_channel_position_mask_t mask)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_cvolume_valid(const pa_cvolume *v)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_cvolume_channels_equal_to(const pa_cvolume *a, pa_volume_t v)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_volume_t
pa_sw_volume_multiply(pa_volume_t a, pa_volume_t b)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume *
pa_sw_cvolume_multiply(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_sw_cvolume_multiply_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t
pa_sw_volume_divide(pa_volume_t a, pa_volume_t b)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume *
pa_sw_cvolume_divide(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_sw_cvolume_divide_scalar(pa_cvolume *dest, const pa_cvolume *a, pa_volume_t b)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_volume_t
pa_sw_volume_from_dB(double f)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
double
pa_sw_volume_to_dB(pa_volume_t v)
{
    trace_info_z("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_remap(pa_cvolume *v, const pa_channel_map *from, const pa_channel_map *to)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_cvolume_compatible(const pa_cvolume *v, const pa_sample_spec *ss)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_cvolume_compatible_with_channel_map(const pa_cvolume *v, const pa_channel_map *cm)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
float
pa_cvolume_get_balance(const pa_cvolume *v, const pa_channel_map *map)
{
    trace_info_z("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_set_balance(pa_cvolume *v, const pa_channel_map *map, float new_balance)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
float
pa_cvolume_get_fade(const pa_cvolume *v, const pa_channel_map *map)
{
    trace_info_z("Z %s\n", __func__);
    return 0.0;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_set_fade(pa_cvolume *v, const pa_channel_map *map, float new_fade)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_scale(pa_cvolume *v, pa_volume_t max)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_scale_mask(pa_cvolume *v, pa_volume_t max, pa_channel_map *cm,
                      pa_channel_position_mask_t mask)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_set_position(pa_cvolume *cv, const pa_channel_map *map, pa_channel_position_t t,
                        pa_volume_t v)
{
    gchar *s_map = trace_pa_channel_map_as_string(map);
    gchar *s_t = trace_pa_channel_position_t_as_string(t);
    trace_info_z("Z %s cv=%p, map=%s, t=%s, v=%u\n", __func__, cv, s_map, s_t, v);
    g_free(s_t);
    g_free(s_map);

    return NULL;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_get_position(pa_cvolume *cv, const pa_channel_map *map, pa_channel_position_t t)
{
    gchar *s_map = trace_pa_channel_map_as_string(map);
    gchar *s_t = trace_pa_channel_position_t_as_string(t);
    trace_info_z("Z %s cv=%p, map=%s, t=%s\n", __func__, cv, s_map, s_t);
    g_free(s_t);
    g_free(s_map);

    return 0;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_merge(pa_cvolume *dest, const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_inc_clamp(pa_cvolume *v, pa_volume_t inc, pa_volume_t limit)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_inc(pa_cvolume *v, pa_volume_t inc)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_dec(pa_cvolume *v, pa_volume_t dec)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_cvolume_equal(const pa_cvolume *a, const pa_cvolume *b)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_init(pa_cvolume *a)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_cvolume_snprint(char *s, size_t l, const pa_cvolume *c)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_sw_cvolume_snprint_dB(char *s, size_t l, const pa_cvolume *c)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}
