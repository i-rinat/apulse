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

#include "util.h"
#include "trace.h"

int
pa_format_to_alsa(pa_sample_format_t format)
{
    switch (format) {
    case PA_SAMPLE_U8:        return SND_PCM_FORMAT_U8;
    case PA_SAMPLE_ALAW:      return SND_PCM_FORMAT_A_LAW;
    case PA_SAMPLE_ULAW:      return SND_PCM_FORMAT_MU_LAW;
    case PA_SAMPLE_S16LE:     return SND_PCM_FORMAT_S16_LE;
    case PA_SAMPLE_S16BE:     return SND_PCM_FORMAT_S16_BE;
    case PA_SAMPLE_FLOAT32LE: return SND_PCM_FORMAT_FLOAT_LE;
    case PA_SAMPLE_FLOAT32BE: return SND_PCM_FORMAT_FLOAT_BE;
    case PA_SAMPLE_S32LE:     return SND_PCM_FORMAT_S32_LE;
    case PA_SAMPLE_S32BE:     return SND_PCM_FORMAT_S32_BE;
    case PA_SAMPLE_S24LE:     return SND_PCM_FORMAT_S24_3LE;
    case PA_SAMPLE_S24BE:     return SND_PCM_FORMAT_S24_3BE;
    case PA_SAMPLE_S24_32LE:  return SND_PCM_FORMAT_S24_LE;
    case PA_SAMPLE_S24_32BE:  return SND_PCM_FORMAT_S24_BE;
    default:                  return SND_PCM_FORMAT_UNKNOWN;
    }
}

pa_sample_format_t
pa_sample_format_from_string(const char *str)
{
    if (!str)
        return 0;

    if (strcmp(str, "u8") == 0) {
        return PA_SAMPLE_U8;
    } else if (strcmp(str, "aLaw") == 0) {
        return PA_SAMPLE_ALAW;
    } else if (strcmp(str, "uLaw") == 0) {
        return PA_SAMPLE_ULAW;
    } else if (strcmp(str, "s16le") == 0) {
        return PA_SAMPLE_S16LE;
    } else if (strcmp(str, "s16be") == 0) {
        return PA_SAMPLE_S16BE;
    } else if (strcmp(str, "float32le") == 0) {
        return PA_SAMPLE_FLOAT32LE;
    } else if (strcmp(str, "float32be") == 0) {
        return PA_SAMPLE_FLOAT32BE;
    } else if (strcmp(str, "s32le") == 0) {
        return PA_SAMPLE_S32LE;
    } else if (strcmp(str, "s32be") == 0) {
        return PA_SAMPLE_S32BE;
    } else if (strcmp(str, "s24le") == 0) {
        return PA_SAMPLE_S24LE;
    } else if (strcmp(str, "s24be") == 0) {
        return PA_SAMPLE_S24BE;
    } else if (strcmp(str, "s24-32le") == 0) {
        return PA_SAMPLE_S24_32LE;
    } else if (strcmp(str, "s24-32be") == 0) {
        return PA_SAMPLE_S24_32BE;
    } else {
        return 0;
    }
}

size_t
pa_find_multiple_of(size_t number, size_t multiple_of, int towards_larger_numbers)
{
    if (multiple_of == 0)
        return number;

    size_t n = towards_larger_numbers ? (number + multiple_of - 1) : number;
    return n - (n % multiple_of);
}

void
pa_apply_volume_multiplier(void *buf, size_t sz, const pa_volume_t volume[PA_CHANNELS_MAX],
                           const pa_sample_spec *ss)
{
    char *p = buf;
    char *last = p + sz;
    float fvol[PA_CHANNELS_MAX];
    uint32_t channels = MIN(ss->channels, PA_CHANNELS_MAX);

    if (channels == 0) {
        // No channels — nothing to scale.
        return;
    }

    int all_normal = 1;
    for (uint32_t k = 0; k < channels; k++)
        all_normal = all_normal && (volume[k] == PA_VOLUME_NORM);

    if (all_normal) {
        // No scaling required.
        return;
    }

    for (uint32_t k = 0; k < channels; k++)
        fvol[k] = pa_sw_volume_to_linear(volume[k]);

    switch (ss->format) {
    case PA_SAMPLE_FLOAT32NE:
        while (p < last) {
            for (uint32_t k = 0; k < channels && p < last; k++) {
                float sample;
                memcpy(&sample, p, sizeof(sample));
                sample *= fvol[k];
                memcpy(p, &sample, sizeof(sample));
                p += sizeof(sample);
            }
        }
        break;

    case PA_SAMPLE_S16NE:
        while (p < last) {
            for (uint32_t k = 0; k < channels && p < last; k++) {
                int16_t sample;
                memcpy(&sample, p, sizeof(sample));
                float sample_scaled = sample * fvol[k];
                sample = CLAMP(sample_scaled, -32768.0, 32767.0);
                memcpy(p, &sample, sizeof(sample));
                p += sizeof(sample);
            }
        }
        break;

    case PA_SAMPLE_U8:
    case PA_SAMPLE_ALAW:
    case PA_SAMPLE_ULAW:
    case PA_SAMPLE_S16RE:
    case PA_SAMPLE_FLOAT32RE:
    case PA_SAMPLE_S32NE:
    case PA_SAMPLE_S32RE:
    case PA_SAMPLE_S24NE:
    case PA_SAMPLE_S24RE:
    case PA_SAMPLE_S24_32NE:
    case PA_SAMPLE_S24_32RE:
    default:
        trace_error("format %s is not implemented in %s\n", pa_sample_format_to_string(ss->format),
                    __func__);
        break;
    }
}
