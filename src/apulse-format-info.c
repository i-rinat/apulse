/*
 * Copyright © 2015-2018  Rinat Ibragimov
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

APULSE_EXPORT
void
pa_format_info_free(pa_format_info *f)
{
    trace_info_f("F %s f=%p\n", __func__, f);
    pa_proplist_free(f->plist);
    free(f);
}

APULSE_EXPORT
pa_format_info *
pa_format_info_new(void)
{
    trace_info_f("F %s (void)\n", __func__);
    pa_format_info *fi = malloc(sizeof(pa_format_info));
    if (!fi)
        return NULL;

    fi->encoding = PA_ENCODING_INVALID;
    fi->plist = pa_proplist_new();

    return fi;
}

APULSE_EXPORT
void
pa_format_info_set_channel_map(pa_format_info *f, const pa_channel_map *map)
{
    gchar *s_map = trace_pa_channel_map_as_string(map);
    trace_info_f("F %s f=%p, map=%s\n", __func__, f, s_map);
    g_free(s_map);

    char buf[PA_CHANNEL_MAP_SNPRINT_MAX];
    pa_channel_map_snprint(buf, sizeof(buf), map);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_CHANNEL_MAP, buf);
}

APULSE_EXPORT
void
pa_format_info_set_channels(pa_format_info *f, int channels)
{
    trace_info_f("F %s f=%p, channels=%d\n", __func__, f, channels);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", channels);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_CHANNELS, buf);
}

APULSE_EXPORT
void
pa_format_info_set_rate(pa_format_info *f, int rate)
{
    trace_info_f("F %s f=%p, rate=%d\n", __func__, f, rate);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", rate);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_RATE, buf);
}

APULSE_EXPORT
void
pa_format_info_set_sample_format(pa_format_info *f, pa_sample_format_t sf)
{
    trace_info_f("F %s f=%p, sf=%u\n", __func__, f, sf);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_SAMPLE_FORMAT,
                     pa_sample_format_to_string(sf));
}

APULSE_EXPORT
int
pa_format_info_valid(const pa_format_info *f)
{
    trace_info_f("F %s f=%p\n", __func__, f);

    int encoding_valid = (f->encoding >= 0 && f->encoding < PA_ENCODING_MAX);
    int plist_valid = (f->plist != NULL);

    return encoding_valid && plist_valid;
}

APULSE_EXPORT
pa_format_info *
pa_format_info_copy(const pa_format_info *src)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_format_info_is_pcm(const pa_format_info *f)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_is_compatible(const pa_format_info *first,
                             const pa_format_info *second)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
char *
pa_format_info_snprint(char *s, size_t l, const pa_format_info *f)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_format_info *
pa_format_info_from_string(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_format_info *
pa_format_info_from_sample_spec(const pa_sample_spec *ss,
                                const pa_channel_map *map)
{
    gchar *s_ss = trace_pa_sample_spec_as_string(ss);
    gchar *s_map = trace_pa_channel_map_as_string(map);
    trace_info_z("Z %s ss=%s, map=%s\n", __func__, s_ss, s_map);
    g_free(s_map);
    g_free(s_ss);

    return NULL;
}

APULSE_EXPORT
int
pa_format_info_to_sample_spec(const pa_format_info *f, pa_sample_spec *ss,
                              pa_channel_map *map)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_prop_type_t
pa_format_info_get_prop_type(const pa_format_info *f, const char *key)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_get_prop_int(const pa_format_info *f, const char *key, int *v)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_get_prop_int_range(const pa_format_info *f, const char *key,
                                  int *min, int *max)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_get_prop_int_array(const pa_format_info *f, const char *key,
                                  int **values, int *n_values)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_get_prop_string(const pa_format_info *f, const char *key,
                               char **v)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_format_info_get_prop_string_array(const pa_format_info *f, const char *key,
                                     char ***values, int *n_values)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
void
pa_format_info_free_string_array(char **values, int n_values)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_format_info_set_prop_int(pa_format_info *f, const char *key, int value)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_format_info_set_prop_int_array(pa_format_info *f, const char *key,
                                  const int *values, int n_values)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_format_info_set_prop_int_range(pa_format_info *f, const char *key, int min,
                                  int max)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_format_info_set_prop_string(pa_format_info *f, const char *key,
                               const char *value)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
void
pa_format_info_set_prop_string_array(pa_format_info *f, const char *key,
                                     const char **values, int n_values)
{
    trace_info_z("Z %s\n", __func__);
}
