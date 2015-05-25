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

#define _XOPEN_SOURCE   500
#include "apulse.h"
#include "trace.h"

static
void *
mainloop_thread(void *param)
{
    pa_threaded_mainloop *m = param;
    int retval;

    pthread_mutex_lock(&m->lock);
    pa_mainloop_run(m->m, &retval);
    pthread_mutex_unlock(&m->lock);

    return NULL;
}

static
int
poll_func(struct pollfd *fds, nfds_t nfds, int timeout, void *userdata)
{
    pthread_mutex_t *lock = userdata;

    pthread_mutex_unlock(lock);
    int ret = poll(fds, nfds, timeout);
    pthread_mutex_lock(lock);

    return ret;
}

APULSE_EXPORT
void
pa_threaded_mainloop_free(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    if (m->running)
        pa_threaded_mainloop_stop(m);

    pthread_mutex_destroy(&m->lock);
    pthread_cond_destroy(&m->cond);
    pa_mainloop_free(m->m);
    free(m);
}

APULSE_EXPORT
pa_mainloop_api *
pa_threaded_mainloop_get_api(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    return &m->m->api;
}

APULSE_EXPORT
int
pa_threaded_mainloop_in_thread(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    return pthread_equal(pthread_self(), m->t);
}

APULSE_EXPORT
void
pa_threaded_mainloop_lock(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    pthread_mutex_lock(&m->lock);
}

APULSE_EXPORT
pa_threaded_mainloop *
pa_threaded_mainloop_new(void)
{
    trace_info("F %s (void)\n", __func__);

    pthread_mutexattr_t mutex_attr;
    pa_threaded_mainloop *m = calloc(1, sizeof(*m));
    m->m = pa_mainloop_new();

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m->lock, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);

    pthread_cond_init(&m->cond, NULL);

    pa_mainloop_set_poll_func(m->m, poll_func, &m->lock);

    return m;
}

APULSE_EXPORT
void
pa_threaded_mainloop_signal(pa_threaded_mainloop *m, int wait_for_accept)
{
    trace_info("F %s m=%p, wait_for_accept=%d\n", __func__, m, wait_for_accept);

    if (wait_for_accept != 0)
        trace_error("%s, not implemented branch\n", __func__);

    pthread_cond_signal(&m->cond);
}

APULSE_EXPORT
int
pa_threaded_mainloop_start(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    if (m->running)
        return 1;
    pthread_create(&m->t, NULL, mainloop_thread, m);
    m->running = 1;
    return 0;
}

APULSE_EXPORT
void
pa_threaded_mainloop_stop(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    pa_mainloop_quit(m->m, 0);
    pthread_join(m->t, NULL);
    m->running = 0;
}

APULSE_EXPORT
void
pa_threaded_mainloop_unlock(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    pthread_mutex_unlock(&m->lock);
}

APULSE_EXPORT
void
pa_threaded_mainloop_wait(pa_threaded_mainloop *m)
{
    trace_info("F %s m=%p\n", __func__, m);

    pthread_cond_wait(&m->cond, &m->lock);
}
