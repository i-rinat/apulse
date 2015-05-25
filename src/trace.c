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

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include "trace.h"


static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void
trace_info(const char *fmt, ...)
{
#ifdef WITH_TRACE
    pthread_mutex_lock(&lock);
    va_list args;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    fprintf(stdout, "%d.%03d [apulse %5d] ", (int)tv.tv_sec, (int)tv.tv_usec/1000, (int)syscall(__NR_gettid));
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    pthread_mutex_unlock(&lock);
#endif
}

void
trace_warning(const char *fmt, ...)
{
    pthread_mutex_lock(&lock);
    va_list args;
    fprintf(stdout, "[apulse] [warning] ");
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    pthread_mutex_unlock(&lock);
}

void
trace_error(const char *fmt, ...)
{
    pthread_mutex_lock(&lock);
    va_list args;
    fprintf(stderr, "[apulse] [error] ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stdout, "[apulse] [error] ");
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    pthread_mutex_unlock(&lock);
}

gchar *
trace_pa_buffer_attr_as_string(const pa_buffer_attr *attr)
{
    gchar *res;
    if (attr) {
        res = g_strdup_printf("{.maxlength=%u, .tlength=%u, .prebuf=%u, .minreq=%u, .fragsize=%u}",
                              attr->maxlength, attr->tlength, attr->prebuf, attr->minreq,
                              attr->fragsize);
    } else {
        res = g_strdup_printf("(nil)");
    }

    return res;
}

void
trace_lock(void)
{
    pthread_mutex_lock(&lock);
}

void
trace_unlock(void)
{
    pthread_mutex_unlock(&lock);
}
