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

#define _XOPEN_SOURCE 500
#include "apulse.h"
#include "trace.h"
#include <assert.h>
#include <unistd.h>

static int
from_pa_io_event_flags(pa_io_event_flags_t flags)
{
    return ((flags & PA_IO_EVENT_INPUT) ? POLLIN : 0) |
           ((flags & PA_IO_EVENT_OUTPUT) ? POLLOUT : 0) |
           ((flags & PA_IO_EVENT_HANGUP) ? POLLHUP : 0) |
           ((flags & PA_IO_EVENT_ERROR) ? POLLERR : 0);
}

static pa_io_event_flags_t
to_pa_io_event_flags(int flags)
{
    return ((flags & POLLIN) ? PA_IO_EVENT_INPUT : 0) |
           ((flags & POLLOUT) ? PA_IO_EVENT_OUTPUT : 0) |
           ((flags & POLLHUP) ? PA_IO_EVENT_HANGUP : 0) |
           ((flags & POLLERR) ? PA_IO_EVENT_ERROR : 0);
}

static void
ml_api_defer_enable(pa_defer_event *e, int b)
{
    trace_info_f("F %s\n", __func__);

    e->enabled = b;
}

static void
ml_api_defer_free(pa_defer_event *e)
{
    trace_info_f("F %s\n", __func__);

    pa_mainloop *ml = e->mainloop;
    g_queue_remove(ml->deferred_events_queue, e);
    g_slice_free(pa_defer_event, e);
    pa_mainloop_wakeup(ml);
}

static pa_defer_event *
ml_api_defer_new(pa_mainloop_api *a, pa_defer_event_cb_t cb, void *userdata)
{
    trace_info_f("F %s\n", __func__);

    pa_mainloop *ml = a->userdata;
    pa_defer_event *de = g_slice_new0(pa_defer_event);
    de->enabled = 1;
    de->cb = cb;
    de->userdata = userdata;
    de->mainloop = ml;
    g_queue_push_tail(ml->deferred_events_queue, de);

    pa_mainloop_wakeup(ml);
    return de;
}

static void
ml_api_defer_set_destroy(pa_defer_event *e, pa_defer_event_destroy_cb_t cb)
{
    trace_info_z("Z %s\n", __func__);
}

static void
ml_api_io_enable(pa_io_event *e, pa_io_event_flags_t events)
{
    trace_info_f("F %s e=%p, events=0x%x\n", __func__, e, events);

    if (e->events == events)
        return;

    e->events = events;

    pa_mainloop *ml = e->mainloop;
    ml->recreate_fds = 1;
    pa_mainloop_wakeup(ml);
}

static void
ml_api_io_free(pa_io_event *e)
{
    trace_info_f("F %s e=%p\n", __func__, e);

    pa_mainloop *ml = e->mainloop;
    g_hash_table_remove(ml->events_ht, e);
    g_slice_free(pa_io_event, e);
    ml->recreate_fds = 1;
    pa_mainloop_wakeup(ml);
}

static pa_io_event *
ml_api_io_new(pa_mainloop_api *a, int fd, pa_io_event_flags_t events,
              pa_io_event_cb_t cb, void *userdata)
{
    trace_info_f("F %s a=%p, fd=%d, events=0x%x, cb=%p, userdata=%p\n",
                 __func__, a, fd, events, cb, userdata);

    pa_mainloop *ml = a->userdata;
    pa_io_event *ioe = g_slice_new(pa_io_event);
    ioe->fd = fd;
    ioe->events = events;
    ioe->cb = cb;
    ioe->cb_userdata = userdata;
    ioe->mainloop = ml;
    ioe->pollfd = NULL;
    ioe->s = NULL;

    g_hash_table_replace(ml->events_ht, ioe, ioe);
    ml->recreate_fds = 1;

    pa_mainloop_wakeup(ml);
    return ioe;
}

static void
ml_api_io_set_destroy(pa_io_event *e, pa_io_event_destroy_cb_t cb)
{
    trace_info_z("Z %s\n", __func__);
}

static void
ml_api_quit(pa_mainloop_api *a, int retval)
{
    trace_info_f("F %s a=%p, retval=%d\n", __func__, a, retval);

    pa_mainloop *ml = a->userdata;
    pa_mainloop_quit(ml, retval);
}

static void
ml_api_time_free(pa_time_event *e)
{
    trace_info_f("F %s e=%p\n", __func__, e);

    pa_mainloop *ml = e->mainloop;
    g_queue_remove(ml->timed_events_queue, e);

    if (e->destroy_cb)
        e->destroy_cb(&ml->api, e, e->userdata);

    g_slice_free(pa_time_event, e);
    pa_mainloop_wakeup(ml);
}

/// Comparator function for |timed_events_queue|. Orders events by value of
/// |when| parameter.
static gint
time_event_comparator(gconstpointer a, gconstpointer b, gpointer user_data)
{
    const pa_time_event *te_a = a;
    const pa_time_event *te_b = b;

    // First, try to compare seconds.
    if (te_a->when.tv_sec < te_b->when.tv_sec)
        return -1;

    if (te_a->when.tv_sec > te_b->when.tv_sec)
        return 1;

    // If we got here, tv_sec fields are equal.

    // Then, compare microseconds.
    if (te_a->when.tv_usec < te_b->when.tv_usec)
        return -1;

    if (te_a->when.tv_usec > te_b->when.tv_usec)
        return 1;

    // Timestamps are equal.
    return 0;
}

static pa_time_event *
ml_api_time_new(pa_mainloop_api *a, const struct timeval *tv,
                pa_time_event_cb_t cb, void *userdata)
{
    trace_info_f("F %s a=%p, tv=%p {%ld, %ld}, cb=%p, userdata=%p\n", __func__,
                 a, tv, tv ? tv->tv_sec : 0, tv ? tv->tv_usec : 0, cb,
                 userdata);

    pa_mainloop *ml = a->userdata;
    pa_time_event *te = g_slice_new0(pa_time_event);
    te->enabled = 1;
    te->when = tv ? *tv : (struct timeval){};
    te->cb = cb;
    te->userdata = userdata;
    te->mainloop = ml;

    g_queue_insert_sorted(ml->timed_events_queue, te, time_event_comparator,
                          NULL);

    pa_mainloop_wakeup(ml);
    return te;
}

static void
ml_api_time_restart(pa_time_event *e, const struct timeval *tv)
{
    trace_info_f("F %s e=%p, tv=%p {%ld, %ld}\n", __func__, e, tv,
                 tv ? tv->tv_sec : 0, tv ? tv->tv_usec : 0);

    pa_mainloop *ml = e->mainloop;

    g_queue_remove(ml->timed_events_queue, e);

    e->enabled = 1;
    e->when = tv ? *tv : (struct timeval){};

    g_queue_insert_sorted(ml->timed_events_queue, e, time_event_comparator,
                          NULL);

    pa_mainloop_wakeup(ml);
}

static void
ml_api_time_set_destroy(pa_time_event *e, pa_time_event_destroy_cb_t cb)
{
    trace_info_f("F %s e=%p, cb=%p\n", __func__, e, cb);

    e->destroy_cb = cb;
}

static void
pa_mainloop_api_once_impl(pa_operation *op)
{
    if (op->mainloop_api_once_cb)
        op->mainloop_api_once_cb(op->api, op->cb_userdata);

    pa_operation_done(op);
}

APULSE_EXPORT
void
pa_mainloop_api_once(pa_mainloop_api *m,
                     void (*callback)(pa_mainloop_api *m, void *userdata),
                     void *userdata)
{
    trace_info_f("F %s\n", __func__);
    pa_operation *op = pa_operation_new(m, pa_mainloop_api_once_impl);
    op->mainloop_api_once_cb = callback;
    op->cb_userdata = userdata;

    pa_operation_launch(op);
}

static int
recover_pcm(snd_pcm_t *pcm)
{
	int ret = 0;
    switch (snd_pcm_state(pcm)) {
    case SND_PCM_STATE_XRUN:
        ret = snd_pcm_recover(pcm, -EPIPE, 1);
        break;
    case SND_PCM_STATE_SUSPENDED:
        ret = snd_pcm_recover(pcm, -ESTRPIPE, 1);
        break;
    case SND_PCM_STATE_DISCONNECTED:
		ret = -1;
		break;
    default:
        snd_pcm_drop(pcm);
        ret = snd_pcm_prepare(pcm);
        break;
    }
	return ret;
}

static long
microseconds_till_event(pa_usec_t now, const struct timeval *event_when)
{
    return (uint64_t)event_when->tv_sec * 1000 * 1000 + event_when->tv_usec -
           now;
}

static void
deferred_stream_restart(pa_mainloop_api *a, pa_defer_event* e, void *userdata)
{
	(void) a;
	(void) e;
    struct pa_io_event *ioe = userdata;

    if (apulse_stream_restart(ioe->s) < 0)
        trace_error("Unrecoverable ALSA PCM error\n");
}

APULSE_EXPORT
int
pa_mainloop_dispatch(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    int cnt = 0;
    GList *keys = g_hash_table_get_keys(m->events_ht);
    GList *it;

    it = keys;
    while (it) {
        struct pa_io_event *ioe = it->data;
        if (ioe->pollfd && ioe->pollfd->revents) {
            int idx = ioe->pollfd - m->fds;
            unsigned short revents = 0;

            if (0 < idx && idx <= m->alsa_special_cnt) {
                snd_pcm_poll_descriptors_revents(ioe->s->ph, ioe->pollfd, 1,
                                                 &revents);
            } else {
                revents = ioe->pollfd->revents;
            }

            if (revents & (~(POLLOUT | POLLIN))) {
                if (recover_pcm(ioe->s->ph) < 0)
                    ml_api_defer_new(&m->api, deferred_stream_restart, ioe);
            } else {
                pa_io_event_flags_t eflags = to_pa_io_event_flags(revents);
                if (ioe->cb)
                    ioe->cb(&m->api, ioe, ioe->fd, eflags, ioe->cb_userdata);
                ioe->pollfd->revents = 0;
                cnt++;
            }
        }

        it = g_list_next(it);
    }
    g_list_free(keys);

    if (m->fds && m->fds[0].revents) {
        // drain wakeup pipe
        char buf[200];
        while (read(m->wakeup_pipe[0], buf, sizeof(buf)) > 0) {
            // cycle
        }
        m->fds[0].revents = 0;
    }

    pa_usec_t now = pa_rtclock_now();
    pa_time_event *te = g_queue_peek_head(m->timed_events_queue);
    while (te && microseconds_till_event(now, &te->when) <= 0) {
        if (te->cb && te->enabled)
            te->cb(&m->api, te, &te->when, te->userdata);
        g_queue_pop_head(m->timed_events_queue);
        te = g_queue_peek_head(m->timed_events_queue);
    }

    pa_defer_event *de = g_queue_pop_head(m->deferred_events_queue);
    while (de) {
        if (de->cb)
            de->cb(&m->api, de, de->userdata);
        de = g_queue_pop_head(m->deferred_events_queue);
    }

    return cnt;
}

APULSE_EXPORT
void
pa_mainloop_free(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    g_queue_free(m->deferred_events_queue);
    g_queue_free(m->timed_events_queue);
    g_hash_table_unref(m->events_ht);
    close(m->wakeup_pipe[0]);
    close(m->wakeup_pipe[1]);
    free(m);
}

APULSE_EXPORT
pa_mainloop_api *
pa_mainloop_get_api(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    return &m->api;
}

APULSE_EXPORT
int
pa_mainloop_get_retval(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    return m->retval;
}

APULSE_EXPORT
int
pa_mainloop_iterate(pa_mainloop *m, int block, int *retval)
{
    trace_info_f("F %s m=%p, block=%d\n", __func__, m, block);

    int err;
    int timeout = block ? -1 : 0;

    err = pa_mainloop_prepare(m, timeout);
    if (err < 0)
        return err;

    err = pa_mainloop_poll(m);
    if (err < 0)
        return err;

    err = pa_mainloop_dispatch(m);

    return err;
}

static void
make_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

APULSE_EXPORT
pa_mainloop *
pa_mainloop_new(void)
{
    trace_info_f("F %s (void)\n", __func__);

    pa_mainloop *m = calloc(1, sizeof(pa_mainloop));
	if (m == NULL)
		return NULL;

    m->api.userdata = m;
    m->api.io_new = ml_api_io_new;
    m->api.io_enable = ml_api_io_enable;
    m->api.io_free = ml_api_io_free;
    m->api.io_set_destroy = ml_api_io_set_destroy;
    m->api.time_new = ml_api_time_new;
    m->api.time_restart = ml_api_time_restart;
    m->api.time_free = ml_api_time_free;
    m->api.time_set_destroy = ml_api_time_set_destroy;
    m->api.defer_new = ml_api_defer_new;
    m->api.defer_enable = ml_api_defer_enable;
    m->api.defer_free = ml_api_defer_free;
    m->api.defer_set_destroy = ml_api_defer_set_destroy;
    m->api.quit = ml_api_quit;

    m->deferred_events_queue = g_queue_new();
    m->timed_events_queue = g_queue_new();
    m->events_ht = g_hash_table_new(g_direct_hash, g_direct_equal);
    m->recreate_fds = 1;

    if (pipe(m->wakeup_pipe) == -1) {
		free(m);
		return NULL;
	}
    make_nonblock(m->wakeup_pipe[0]);
    make_nonblock(m->wakeup_pipe[1]);

    return m;
}

APULSE_EXPORT
int
pa_mainloop_poll(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    long int timeout = m->timeout;

    pa_time_event *te = g_queue_peek_head(m->timed_events_queue);
    if (te) {
        pa_usec_t now = pa_rtclock_now();
        long int msecs_till_next_event =
            microseconds_till_event(now, &te->when) / PA_USEC_PER_MSEC;

        // Ensure delay is non-negative, even if event is already expired.
        msecs_till_next_event = MAX(msecs_till_next_event, 0);

        if (timeout < 0) {
            // poll() call was supposed to wait for indefinite period of time.
            timeout = msecs_till_next_event;

        } else {
            timeout = MIN(timeout, msecs_till_next_event);
        }

        // |timeout| value should fit int limits.
        timeout = MIN(timeout, INT32_MAX);
    }

    int ret;
    if (m->poll_func) {
        ret = m->poll_func(m->fds, m->nfds, timeout, m->poll_func_userdata);
    } else {
        ret = poll(m->fds, m->nfds, timeout);
    }

    return ret;
}

APULSE_EXPORT
int
pa_mainloop_prepare(pa_mainloop *m, int timeout)
{
    trace_info_f("P %s m=%p, timeout=%d\n", __func__, m, timeout);

    m->timeout = timeout;
    if (m->recreate_fds) {
        GList *keys = g_hash_table_get_keys(m->events_ht);
        GList *it;
        struct pollfd *tmp;

        m->nfds = g_list_length(keys) + 1;
        tmp = realloc(m->fds, m->nfds * sizeof(*m->fds));
        if (!tmp)
            return -1;
        m->fds = tmp;

        m->fds[0].fd = m->wakeup_pipe[0];
        m->fds[0].events = POLLIN;
        m->fds[0].revents = 0;

        /* special case for alsa pollfds */
        int k = 1;
        m->alsa_special_cnt = 0;
        it = keys;
        while (it) {
            struct pa_io_event *ioe = it->data;
            if (ioe->events & 0x80000000u) {
                m->fds[k].fd = ioe->fd;
                m->fds[k].events = ioe->events & (~0x80000000u);
                m->fds[k].revents = 0;
                ioe->pollfd = &m->fds[k];
                k++;
                m->alsa_special_cnt++;
            }
            it = g_list_next(it);
        }

        /* normal file descriptors */
        it = keys;
        while (it) {
            struct pa_io_event *ioe = it->data;
            if ((ioe->events & 0x80000000u) == 0) {
                m->fds[k].fd = ioe->fd;
                m->fds[k].events = from_pa_io_event_flags(ioe->events);
                m->fds[k].revents = 0;
                ioe->pollfd = &m->fds[k];
                k++;
            }
            it = g_list_next(it);
        }

        m->nfds = k;

        g_list_free(keys);
        m->recreate_fds = 0;
    }

    return 0;
}

APULSE_EXPORT
void
pa_mainloop_quit(pa_mainloop *m, int r)
{
    trace_info_f("F %s m=%p, r=%d\n", __func__, m, r);

    m->retval = r;
    m->terminate = 1;
    pa_mainloop_wakeup(m);
}

APULSE_EXPORT
int
pa_mainloop_run(pa_mainloop *m, int *retval)
{
    trace_info_f("F %s\n", __func__);

    int ret;
    do {
        ret = pa_mainloop_iterate(m, 1, retval);
    } while (ret >= 0 && !m->terminate);

    if (m->terminate) {
        *retval = m->retval;
        return 0;
    }

    return ret;
}

APULSE_EXPORT
void
pa_mainloop_set_poll_func(pa_mainloop *m, pa_poll_func poll_func,
                          void *userdata)
{
    trace_info_f("F %s m=%p, poll_func=%p, userdata=%p\n", __func__, m,
                 poll_func, userdata);

    m->poll_func = poll_func;
    m->poll_func_userdata = userdata;
}

APULSE_EXPORT
void
pa_mainloop_wakeup(pa_mainloop *m)
{
    trace_info_f("F %s m=%p\n", __func__, m);

    char c = '!';
    if (write(m->wakeup_pipe[1], &c, 1) != 1)
		trace_error("Unable to write to wakeup pipe: %s\n", strerror(errno));
}
