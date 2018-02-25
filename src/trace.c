/*
 * Copyright © 2014-2018  Rinat Ibragimov
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

#include "trace.h"
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#if CONFIG_LOG_TO_STDERR
static int log_to_stderr = 1;
#else
static int log_to_stderr = 0;
#endif  // CONFIG_LOG_TO_STDERR

void
trace_info(const char *fmt, ...)
{
    FILE *fp = log_to_stderr ? stderr : stdout;
    pthread_mutex_lock(&lock);
    va_list args;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    fprintf(fp, "%d.%03d [apulse %5d] ", (int)tv.tv_sec,
            (int)tv.tv_usec / 1000, (int)syscall(__NR_gettid));
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    pthread_mutex_unlock(&lock);
}

void
trace_warning(const char *fmt, ...)
{
    FILE *fp = log_to_stderr ? stderr : stdout;
    pthread_mutex_lock(&lock);
    va_list args;
    fprintf(fp, "[apulse] [warning] ");
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    pthread_mutex_unlock(&lock);
}

void
trace_error(const char *fmt, ...)
{
    static int stdout_tested = 0;
    static int stdout_is_a_tty = 0;

    pthread_mutex_lock(&lock);

    if (!stdout_tested) {
        stdout_is_a_tty = isatty(1);
        stdout_tested = 1;
    }

    va_list args;
    fprintf(stderr, "[apulse] [error] ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (!log_to_stderr && !stdout_is_a_tty) {
        // Send a copy to stdout, if it's redirected to a file.
        fprintf(stdout, "[apulse] [error] ");
        va_start(args, fmt);
        vfprintf(stdout, fmt, args);
        va_end(args);
    }

    pthread_mutex_unlock(&lock);
}

gchar *
trace_pa_buffer_attr_as_string(const pa_buffer_attr *attr)
{
    gchar *res;
    if (attr) {
        res = g_strdup_printf(
            "{.maxlength=%u, .tlength=%u, .prebuf=%u, .minreq=%u, "
            ".fragsize=%u}",
            attr->maxlength, attr->tlength, attr->prebuf, attr->minreq,
            attr->fragsize);
    } else {
        res = g_strdup_printf("(nil)");
    }

    return res;
}

gchar *
trace_pa_volume_as_string(const pa_cvolume *v)
{
    const unsigned int channel_count = MIN(v->channels, PA_CHANNELS_MAX);
    GString *s = g_string_new(NULL);

    g_string_append_printf(s, "%d:{", v->channels);
    for (unsigned int k = 0; k < channel_count; k++) {
        if (k != 0)
            g_string_append(s, ", ");

        g_string_append_printf(s, "%u", v->values[k]);
    }

    g_string_append(s, "}");

    return g_string_free(s, FALSE);
}

gchar *
trace_pa_channel_position_t_as_string(const pa_channel_position_t pos)
{
    const char *s_pos;

    switch (pos) {
    case PA_CHANNEL_POSITION_INVALID:
        s_pos = "INVALID";
        break;

    case PA_CHANNEL_POSITION_MONO:
        s_pos = "MONO";
        break;

    case PA_CHANNEL_POSITION_FRONT_LEFT:
        s_pos = "FRONT_LEFT";
        break;

    case PA_CHANNEL_POSITION_FRONT_RIGHT:
        s_pos = "FRONT_RIGHT";
        break;

    case PA_CHANNEL_POSITION_FRONT_CENTER:
        s_pos = "FRONT_CENTER";
        break;

    case PA_CHANNEL_POSITION_REAR_CENTER:
        s_pos = "REAR_CENTER";
        break;

    case PA_CHANNEL_POSITION_REAR_LEFT:
        s_pos = "REAR_LEFT";
        break;

    case PA_CHANNEL_POSITION_REAR_RIGHT:
        s_pos = "REAR_RIGHT";
        break;

    case PA_CHANNEL_POSITION_LFE:
        s_pos = "LFE";
        break;

    case PA_CHANNEL_POSITION_FRONT_LEFT_OF_CENTER:
        s_pos = "FRONT_LEFT_OF_CENTER";
        break;

    case PA_CHANNEL_POSITION_FRONT_RIGHT_OF_CENTER:
        s_pos = "FRONT_RIGHT_OF_CENTER";
        break;

    case PA_CHANNEL_POSITION_SIDE_LEFT:
        s_pos = "SIDE_LEFT";
        break;

    case PA_CHANNEL_POSITION_SIDE_RIGHT:
        s_pos = "SIDE_RIGHT";
        break;

    case PA_CHANNEL_POSITION_AUX0:
        s_pos = "AUX0";
        break;

    case PA_CHANNEL_POSITION_AUX1:
        s_pos = "AUX1";
        break;

    case PA_CHANNEL_POSITION_AUX2:
        s_pos = "AUX2";
        break;

    case PA_CHANNEL_POSITION_AUX3:
        s_pos = "AUX3";
        break;

    case PA_CHANNEL_POSITION_AUX4:
        s_pos = "AUX4";
        break;

    case PA_CHANNEL_POSITION_AUX5:
        s_pos = "AUX5";
        break;

    case PA_CHANNEL_POSITION_AUX6:
        s_pos = "AUX6";
        break;

    case PA_CHANNEL_POSITION_AUX7:
        s_pos = "AUX7";
        break;

    case PA_CHANNEL_POSITION_AUX8:
        s_pos = "AUX8";
        break;

    case PA_CHANNEL_POSITION_AUX9:
        s_pos = "AUX9";
        break;

    case PA_CHANNEL_POSITION_AUX10:
        s_pos = "AUX10";
        break;

    case PA_CHANNEL_POSITION_AUX11:
        s_pos = "AUX11";
        break;

    case PA_CHANNEL_POSITION_AUX12:
        s_pos = "AUX12";
        break;

    case PA_CHANNEL_POSITION_AUX13:
        s_pos = "AUX13";
        break;

    case PA_CHANNEL_POSITION_AUX14:
        s_pos = "AUX14";
        break;

    case PA_CHANNEL_POSITION_AUX15:
        s_pos = "AUX15";
        break;

    case PA_CHANNEL_POSITION_AUX16:
        s_pos = "AUX16";
        break;

    case PA_CHANNEL_POSITION_AUX17:
        s_pos = "AUX17";
        break;

    case PA_CHANNEL_POSITION_AUX18:
        s_pos = "AUX18";
        break;

    case PA_CHANNEL_POSITION_AUX19:
        s_pos = "AUX19";
        break;

    case PA_CHANNEL_POSITION_AUX20:
        s_pos = "AUX20";
        break;

    case PA_CHANNEL_POSITION_AUX21:
        s_pos = "AUX21";
        break;

    case PA_CHANNEL_POSITION_AUX22:
        s_pos = "AUX22";
        break;

    case PA_CHANNEL_POSITION_AUX23:
        s_pos = "AUX23";
        break;

    case PA_CHANNEL_POSITION_AUX24:
        s_pos = "AUX24";
        break;

    case PA_CHANNEL_POSITION_AUX25:
        s_pos = "AUX25";
        break;

    case PA_CHANNEL_POSITION_AUX26:
        s_pos = "AUX26";
        break;

    case PA_CHANNEL_POSITION_AUX27:
        s_pos = "AUX27";
        break;

    case PA_CHANNEL_POSITION_AUX28:
        s_pos = "AUX28";
        break;

    case PA_CHANNEL_POSITION_AUX29:
        s_pos = "AUX29";
        break;

    case PA_CHANNEL_POSITION_AUX30:
        s_pos = "AUX30";
        break;

    case PA_CHANNEL_POSITION_AUX31:
        s_pos = "AUX31";
        break;

    case PA_CHANNEL_POSITION_TOP_CENTER:
        s_pos = "TOP_CENTER";
        break;

    case PA_CHANNEL_POSITION_TOP_FRONT_LEFT:
        s_pos = "TOP_FRONT_LEFT";
        break;

    case PA_CHANNEL_POSITION_TOP_FRONT_RIGHT:
        s_pos = "TOP_FRONT_RIGHT";
        break;

    case PA_CHANNEL_POSITION_TOP_FRONT_CENTER:
        s_pos = "TOP_FRONT_CENTER";
        break;

    case PA_CHANNEL_POSITION_TOP_REAR_LEFT:
        s_pos = "TOP_REAR_LEFT";
        break;

    case PA_CHANNEL_POSITION_TOP_REAR_RIGHT:
        s_pos = "TOP_REAR_RIGHT";
        break;

    case PA_CHANNEL_POSITION_TOP_REAR_CENTER:
        s_pos = "TOP_REAR_CENTER";
        break;

    case PA_CHANNEL_POSITION_MAX:
        s_pos = "MAX";
        break;

    default:
        s_pos = "UNKNOWN";
        break;
    }

    return g_strdup_printf("%s(%d)", s_pos, pos);
}

gchar *
trace_pa_channel_map_as_string(const pa_channel_map *m)
{
    if (!m)
        return g_strdup("(nil)");

    GString *s = g_string_new(NULL);

    g_string_append_printf(s, "%u:{", m->channels);

    const unsigned int channel_count = MIN(m->channels, PA_CHANNELS_MAX);
    for (unsigned int k = 0; k < channel_count; k++) {
        if (k != 0)
            g_string_append(s, ",");
        gchar *s_channel_pos = trace_pa_channel_position_t_as_string(m->map[k]);
        g_string_append(s, s_channel_pos);
        g_free(s_channel_pos);
    }

    g_string_append(s, "}");

    return g_string_free(s, FALSE);
}

gchar *
trace_pa_sample_format_t_as_string(pa_sample_format_t sf)
{
    const char *fmt;
    switch (sf) {
    case PA_SAMPLE_U8:
        fmt = "U8";
        break;

    case PA_SAMPLE_ALAW:
        fmt = "ALAW";
        break;

    case PA_SAMPLE_ULAW:
        fmt = "ULAW";
        break;

    case PA_SAMPLE_S16LE:
        fmt = "S16LE";
        break;

    case PA_SAMPLE_S16BE:
        fmt = "S16BE";
        break;

    case PA_SAMPLE_FLOAT32LE:
        fmt = "FLOAT32LE";
        break;

    case PA_SAMPLE_FLOAT32BE:
        fmt = "FLOAT32BE";
        break;

    case PA_SAMPLE_S32LE:
        fmt = "S32LE";
        break;

    case PA_SAMPLE_S32BE:
        fmt = "S32BE";
        break;

    case PA_SAMPLE_S24LE:
        fmt = "S24LE";
        break;

    case PA_SAMPLE_S24BE:
        fmt = "S24BE";
        break;

    case PA_SAMPLE_S24_32LE:
        fmt = "S24_32LE";
        break;

    case PA_SAMPLE_S24_32BE:
        fmt = "S24_32BE";
        break;

    case PA_SAMPLE_MAX:
        fmt = "MAX";
        break;

    case PA_SAMPLE_INVALID:
        fmt = "INVALID";
        break;

    default:
        fmt = "UNKNOWN";
        break;
    }

    return g_strdup_printf("%s(%d)", fmt, sf);
}

gchar *
trace_pa_sample_spec_as_string(const pa_sample_spec *ss)
{
    if (!ss)
        return g_strdup("(nil)");

    gchar *s_format = trace_pa_sample_format_t_as_string(ss->format);
    gchar *str = g_strdup_printf("{.format=%s,.rate=%u,.channels=%u}", s_format,
                                 ss->rate, ss->channels);
    g_free(s_format);

    return str;
}
