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
    trace_info("Z %s\n", __func__);
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
