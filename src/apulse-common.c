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

typedef enum pa_log_level {
    PA_LOG_ERROR  = 0,
    PA_LOG_WARN   = 1,
    PA_LOG_NOTICE = 2,
    PA_LOG_INFO   = 3,
    PA_LOG_DEBUG  = 4,
    PA_LOG_LEVEL_MAX
} pa_log_level_t;

APULSE_EXPORT
void
pa_log_level_meta(pa_log_level_t level, const char *file, int line, const char *func,
                  const char *format, ...)
{
#ifdef WITH_TRACE
    trace_lock();
    va_list args;
    fprintf(stdout, "pa_log: <%d> %s:%d %s ", level, file, line, func);
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fprintf(stdout, "\n");
    trace_unlock();
#endif
}

APULSE_EXPORT
void
pa_disable_sigpipe(void)
{
    trace_info_z("Z %s\n", __func__);
}

APULSE_EXPORT
int
pa_open_cloexec(const char *fn, int flags, mode_t mode)
{
    int fd = open(fn, flags, mode);
    if (fd < 0)
        return fd;
    fcntl(fd, F_SETFD, fcntl(fd, F_GETFD, 0) | FD_CLOEXEC);
    return fd;
}

APULSE_EXPORT
int
pa_close(int fd)
{
    close(fd);
    return 0;
}

APULSE_EXPORT
char *
pa_ascii_filter(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_ascii_valid(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_utf8_filter(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_utf8_to_locale(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_utf8_valid(const char *str)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_bytes_snprint(char *s, size_t l, unsigned v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
const char *
pa_encoding_to_string(pa_encoding_t e)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_encoding_t
pa_encoding_from_string(const char *encoding)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
char *
pa_get_fqdn(char *s, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_get_home_dir(char *s, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_msleep(unsigned long t)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_usec_t
pa_timeval_diff(const struct timeval *a, const struct timeval *b)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_timeval_cmp(const struct timeval *a, const struct timeval *b)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_usec_t
pa_timeval_age(const struct timeval *tv)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
struct timeval *
pa_timeval_add(struct timeval *tv, pa_usec_t v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
struct timeval *
pa_timeval_sub(struct timeval *tv, pa_usec_t v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
struct timeval *
pa_timeval_store(struct timeval *tv, pa_usec_t v)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
pa_usec_t
pa_timeval_load(const struct timeval *tv)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
char *
pa_get_host_name(char *s, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_get_user_name(char *s, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
struct timeval *
pa_gettimeofday(struct timeval *tv)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void *
pa_xmemdup(const void *p, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
void *
pa_xrealloc(void *ptr, size_t size)
{
    trace_info_f("F %s ptr=%p, size=%zu\n", __func__, ptr, size);

    void *p = realloc(ptr, size);
    if (!p)
        return ptr;

    return p;
}

APULSE_EXPORT
char *
pa_xstrdup(const char *s)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
char *
pa_xstrndup(const char *s, size_t l)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_sample_format_is_le(pa_sample_format_t f)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
int
pa_sample_format_is_be(pa_sample_format_t f)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
size_t
pa_sample_size_of_format(pa_sample_format_t f)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}

APULSE_EXPORT
pa_sample_spec *
pa_sample_spec_init(pa_sample_spec *spec)
{
    trace_info_z("Z %s\n", __func__);
    return NULL;
}

APULSE_EXPORT
int
pa_sample_spec_equal(const pa_sample_spec *a, const pa_sample_spec *b)
{
    gchar *s_a = trace_pa_sample_spec_as_string(a);
    gchar *s_b = trace_pa_sample_spec_as_string(b);
    trace_info_z("Z %s a=%s, b=%s\n", __func__, s_a, s_b);
    g_free(s_b);
    g_free(s_a);

    return 0;
}

APULSE_EXPORT
pa_sample_format_t
pa_parse_sample_format(const char *format)
{
    trace_info_z("Z %s\n", __func__);
    return 0;
}
