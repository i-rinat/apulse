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


APULSE_EXPORT
void
pa_format_info_free(pa_format_info *f)
{
    trace_info("F %s f=%p\n", __func__, f);
    pa_proplist_free(f->plist);
    free(f);
}

APULSE_EXPORT
pa_format_info *
pa_format_info_new(void)
{
    trace_info("F %s (void)\n", __func__);
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
    trace_info("F %s f=%p, map=%p\n", __func__, f, map);
    char buf[PA_CHANNEL_MAP_SNPRINT_MAX];
    pa_channel_map_snprint(buf, sizeof(buf), map);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_CHANNEL_MAP, buf);
}

APULSE_EXPORT
void pa_format_info_set_channels(pa_format_info *f, int channels)
{
    trace_info("F %s f=%p, channels=%d\n", __func__, f, channels);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", channels);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_CHANNELS, buf);
}

APULSE_EXPORT
void pa_format_info_set_rate(pa_format_info *f, int rate)
{
    trace_info("F %s f=%p, rate=%d\n", __func__, f, rate);
    char buf[20];
    snprintf(buf, sizeof(buf), "%d", rate);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_RATE, buf);
}

APULSE_EXPORT
void
pa_format_info_set_sample_format(pa_format_info *f, pa_sample_format_t sf)
{
    trace_info("F %s f=%p, sf=%u\n", __func__, f, sf);
    pa_proplist_sets(f->plist, PA_PROP_FORMAT_SAMPLE_FORMAT, pa_sample_format_to_string(sf));
}

APULSE_EXPORT
int pa_format_info_valid(const pa_format_info *f)
{
    trace_info("F %s f=%p\n", __func__, f);

    int encoding_valid = (f->encoding >= 0 && f->encoding < PA_ENCODING_MAX);
    int plist_valid =    (f->plist != NULL);

    return encoding_valid && plist_valid;
}
