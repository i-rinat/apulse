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
