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

#pragma once

#include <glib.h>
#include <pulse/channelmap.h>
#include <pulse/pulseaudio.h>

#if CONFIG_TRACE_LEVEL >= 2

#define trace_info_f(...) trace_info(__VA_ARGS__)
#define trace_info_z(...) trace_info(__VA_ARGS__)

#elif CONFIG_TRACE_LEVEL == 1

#define trace_info_f(...)
#define trace_info_z(...) trace_info(__VA_ARGS__)

#else  // CONFIG_TRACE_LEVEL == 0

#define trace_info_f(...)
#define trace_info_z(...)

#endif

void
trace_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

void
trace_warning(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

void
trace_error(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

gchar *
trace_pa_buffer_attr_as_string(const pa_buffer_attr *attr);

gchar *
trace_pa_volume_as_string(const pa_cvolume *v);

gchar *
trace_pa_channel_position_t_as_string(const pa_channel_position_t pos);

gchar *
trace_pa_channel_map_as_string(const pa_channel_map *m);

gchar *
trace_pa_sample_format_t_as_string(pa_sample_format_t sf);

gchar *
trace_pa_sample_spec_as_string(const pa_sample_spec *ss);
