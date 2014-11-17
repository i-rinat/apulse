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
#include "util.h"


APULSE_EXPORT
const char *
pa_get_library_version(void)
{
    trace_info("F %s (void)\n", __func__);

    return pa_get_headers_version();
}

APULSE_EXPORT
int
pa_sample_format_valid(unsigned format)
{
    return format < PA_SAMPLE_MAX;
}

APULSE_EXPORT
int
pa_sample_rate_valid(uint32_t rate)
{
    return 0 < rate && rate <= PA_RATE_MAX;
}

APULSE_EXPORT
int
pa_channels_valid(uint8_t channels)
{
    return 0 < channels && channels <= PA_CHANNELS_MAX;
}

APULSE_EXPORT
int
pa_sample_spec_valid(const pa_sample_spec *spec)
{
    return spec && pa_sample_format_valid(spec->format)
                && pa_sample_rate_valid(spec->rate)
                && pa_channels_valid(spec->channels);
}

APULSE_EXPORT
const char *
pa_strerror(int error)
{
    trace_info("P %s error=%d\n", __func__, error);

    return "Oops.";
}

APULSE_EXPORT
size_t
pa_usec_to_bytes(pa_usec_t t, const pa_sample_spec *spec)
{
    return t * pa_bytes_per_second(spec) / (1000 * 1000);
}

APULSE_EXPORT
pa_usec_t
pa_bytes_to_usec(uint64_t length, const pa_sample_spec *spec)
{
    return 1000 * 1000 * length / pa_bytes_per_second(spec);
}

APULSE_EXPORT
size_t
pa_bytes_per_second(const pa_sample_spec *spec)
{
    return spec->rate * pa_frame_size(spec);
}

APULSE_EXPORT
size_t
pa_frame_size(const pa_sample_spec *spec)
{
    return spec->channels * pa_sample_size(spec);
}

APULSE_EXPORT
size_t
pa_sample_size(const pa_sample_spec *spec)
{
    switch (spec->format) {
    case PA_SAMPLE_U8:
    case PA_SAMPLE_ALAW:
    case PA_SAMPLE_ULAW:
        return 1;

    case PA_SAMPLE_S16LE:
    case PA_SAMPLE_S16BE:
        return 2;

    case PA_SAMPLE_S24LE:
    case PA_SAMPLE_S24BE:
        return 3;

    case PA_SAMPLE_FLOAT32LE:
    case PA_SAMPLE_FLOAT32BE:
    case PA_SAMPLE_S32LE:
    case PA_SAMPLE_S32BE:
    case PA_SAMPLE_S24_32LE:
    case PA_SAMPLE_S24_32BE:
    case PA_SAMPLE_MAX:
        return 4;

    case PA_SAMPLE_INVALID:
    default:
        return 0;
    }
}

APULSE_EXPORT
pa_cvolume *
pa_cvolume_set(pa_cvolume *a, unsigned channels, pa_volume_t v)
{
    trace_info("Z %s a=%p, channels=%u, v=%u\n", __func__, a, channels, v);

    return NULL;
}

APULSE_EXPORT
pa_volume_t
pa_cvolume_avg(const pa_cvolume *a)
{
    trace_info("Z %s a=%p\n", __func__, a);

    return 0;
}

APULSE_EXPORT
const char *
pa_sample_format_to_string(pa_sample_format_t format)
{
    switch (format) {
    case PA_SAMPLE_U8:        return "u8";
    case PA_SAMPLE_ALAW:      return "aLaw";
    case PA_SAMPLE_ULAW:      return "uLaw";
    case PA_SAMPLE_S16LE:     return "s16le";
    case PA_SAMPLE_S16BE:     return "s16be";
    case PA_SAMPLE_FLOAT32LE: return "float32le";
    case PA_SAMPLE_FLOAT32BE: return "float32be";
    case PA_SAMPLE_S32LE:     return "s32le";
    case PA_SAMPLE_S32BE:     return "s32be";
    case PA_SAMPLE_S24LE:     return "s24le";
    case PA_SAMPLE_S24BE:     return "s24be";
    case PA_SAMPLE_S24_32LE:  return "s24-32le";
    case PA_SAMPLE_S24_32BE:  return "s24-32be";
    default:                  return NULL;
    }
}

APULSE_EXPORT
char *
pa_sample_spec_snprint(char *s, size_t l, const pa_sample_spec *spec)
{
    trace_info("F %s s=%p, l=%u, spec=%p\n", __func__, s, (unsigned)l, spec);

    if (spec && pa_sample_spec_valid(spec)) {
        snprintf(s, l, "%s %uch %uHz", pa_sample_format_to_string(spec->format), spec->channels,
                 spec->rate);
    } else {
        snprintf(s, l, "invalid");
    }

    return s;
}

APULSE_EXPORT
char *
pa_path_get_filename(const char *p)
{
    char *last_slash;
    if (!p)
        return NULL;
    last_slash = strrchr(p, '/');
    if (!last_slash)
        return (char *)p;
    return last_slash + 1;
}

APULSE_EXPORT
void
pa_xfree(void *p)
{
    free(p);
}

APULSE_EXPORT
void *
pa_xmalloc(size_t size)
{
    return malloc(size);
}

APULSE_EXPORT
void *
pa_xmalloc0(size_t l)
{
    return calloc(l, 1);
}

APULSE_EXPORT
char *
pa_locale_to_utf8(const char *str)
{
    trace_info("Z %s\n", __func__);

    return strdup(str);
}
