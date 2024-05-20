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

#define _GNU_SOURCE
#include "ringbuffer.h"
#include <alsa/asoundlib.h>
#include <glib.h>
#include <poll.h>
#include <pthread.h>
#include <pulse/pulseaudio.h>

#define APULSE_EXPORT __attribute__((visibility("default")))

struct pa_context {
    pa_context_state_t state;
    pa_context_state_t new_state;
    pa_context_notify_cb_t state_cb;
    void *state_cb_userdata;
    pa_mainloop_api *mainloop_api;
    char *name;
    int ref_cnt;
    int next_stream_idx;
    GHashTable *streams_ht;
    pa_volume_t source_volume[PA_CHANNELS_MAX];
};

struct pa_io_event {
    int fd;
    pa_io_event_flags_t events;
    pa_io_event_cb_t cb;
    void *cb_userdata;
    pa_io_event_destroy_cb_t destroy_cb;
    pa_mainloop *mainloop;
    struct pollfd *pollfd;
    pa_stream *s;
};

struct pa_mainloop {
    pa_mainloop_api api;
    GQueue *deferred_events_queue;
    GQueue *timed_events_queue;
    GHashTable *events_ht;  ///< a set of (pa_io_event *)
    struct pollfd *fds;
    nfds_t nfds;
    int recreate_fds;  ///< 1 if fds array needs to be recreated from events_ht
    int timeout;
    int wakeup_pipe[2];
    int terminate;
    int retval;
    pa_poll_func poll_func;
    void *poll_func_userdata;
    int alsa_special_cnt;
};

struct pa_glib_mainloop {
    pa_mainloop_api api;
};

struct pa_threaded_mainloop {
    pa_mainloop *m;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    pthread_t t;
    int running;
};

struct pa_proplist {
    GHashTable *ht;
};

struct pa_stream {
    pa_context *c;
    pa_stream_state_t state;
    pa_stream_direction_t direction;
    char *requested_device_name;
    snd_pcm_t *ph;
    pa_sample_spec ss;
    pa_buffer_attr buffer_attr;
    int ref_cnt;
    int idx;
    pa_stream_notify_cb_t state_cb;
    void *state_cb_userdata;
    pa_stream_request_cb_t write_cb;
    void *write_cb_userdata;
    pa_stream_request_cb_t read_cb;
    void *read_cb_userdata;
    pa_stream_notify_cb_t latency_update_cb;
    void *latency_update_cb_userdata;
    char *name;
    pa_timing_info timing_info;
    pa_io_event **ioe;  ///< list of pa_io_events
    int nioe;           ///< count of pa_io_events
    ringbuffer_t *rb;
    void *peek_buffer;
    size_t peek_buffer_data_len;
    void *write_buffer;
    volatile int paused;
    pa_volume_t volume[PA_CHANNELS_MAX];
	pa_time_event *cork_timer;
	uint64_t cork_latency_bytes;
};

struct pa_operation {
    pa_operation_state_t state;
    pa_stream_success_cb_t stream_success_cb;
    pa_sink_input_info_cb_t sink_input_info_cb;
    pa_sink_info_cb_t sink_info_cb;
    pa_context_success_cb_t context_success_cb;
    pa_server_info_cb_t server_info_cb;
    pa_source_info_cb_t source_info_cb;
    void *cb_userdata;

    pa_mainloop_api *api;

    void (*mainloop_api_once_cb)(pa_mainloop_api *m, void *userdata);

    void (*handler)(pa_operation *op);

    int ref_cnt;
    int int_arg_1;
    char *char_ptr_arg_1;

    pa_cvolume pa_cvolume_arg_1;

    pa_stream *s;
    pa_context *c;
};

struct pa_defer_event {
    int enabled;
    pa_defer_event_cb_t cb;
    void *userdata;
    pa_mainloop *mainloop;
};

struct pa_time_event {
    int enabled;
    struct timeval when;
    pa_time_event_cb_t cb;
    void *userdata;
    pa_mainloop *mainloop;
    pa_time_event_destroy_cb_t destroy_cb;
};

struct pa_simple {
    pa_context *context;
    pa_threaded_mainloop *mainloop;
    pa_stream *stream;
    pa_stream_direction_t direction;
    int initialized;
};

pa_operation *
pa_operation_new(pa_mainloop_api *api, void (*handler)(pa_operation *op));

void
pa_operation_launch(pa_operation *op);

void
pa_operation_done(pa_operation *op);

int
apulse_stream_restart(pa_stream *s);
