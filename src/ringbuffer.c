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

#include "ringbuffer.h"
#include <stdlib.h>
#include <string.h>


static
inline
void
_rb_lock(ringbuffer_t *rb)
{
    pthread_mutex_lock(&rb->lock);
}

static
inline
void
_rb_unlock(ringbuffer_t *rb)
{
    pthread_mutex_unlock(&rb->lock);
}

ringbuffer_t *
ringbuffer_new(size_t sz)
{
    ringbuffer_t *rb = malloc(sizeof(*rb));

    rb->start = malloc(sz);
    rb->end = rb->start + sz;
    rb->rpos = rb->start;
    rb->wpos = rb->start;
    rb->empty = 1;
    pthread_mutex_init(&rb->lock, NULL);

    return rb;
}

void
ringbuffer_free(ringbuffer_t *rb)
{
    pthread_mutex_destroy(&rb->lock);
    free(rb->start);
    free(rb);
}

static
size_t
_ringbuffer_readable_size(ringbuffer_t *rb)
{
    if (rb->wpos > rb->rpos) {
        return rb->wpos - rb->rpos;
    } else if (rb->wpos < rb->rpos) {
        return (rb->end - rb->rpos) + (rb->wpos - rb->start);
    } else {
        // wpos == rpos
        if (rb->empty)
            return 0;
        else
            return rb->end - rb->start;
    }
}

size_t
ringbuffer_readable_size(ringbuffer_t *rb)
{
    size_t rsz;

    _rb_lock(rb);
    rsz = _ringbuffer_readable_size(rb);
    _rb_unlock(rb);

    return rsz;
}

static
size_t
_ringbuffer_writable_size(ringbuffer_t *rb)
{
    if (rb->wpos > rb->rpos) {
        return (rb->end - rb->wpos) + (rb->rpos - rb->start);
    } else if (rb->wpos < rb->rpos) {
        return rb->rpos - rb->wpos;
    } else {
        // wpos == rpos
        if (rb->empty)
            return rb->end - rb->start;
        else
            return 0;
    }
}

size_t
ringbuffer_writable_size(ringbuffer_t *rb)
{
    size_t wsz;

    _rb_lock(rb);
    wsz = _ringbuffer_writable_size(rb);
    _rb_unlock(rb);

    return wsz;
}

size_t
ringbuffer_write(ringbuffer_t *rb, const void *data, size_t len)
{
    _rb_lock(rb);

    size_t remsz = rb->end - rb->wpos;
    size_t writable_size = _ringbuffer_writable_size(rb);

    if (len > writable_size)
        len = writable_size;

    if (len <= remsz) {
        memcpy(rb->wpos, data, len);
        rb->wpos += len;
    } else {
        memcpy(rb->wpos, data, remsz);
        memcpy(rb->start, (char *)data + remsz, len - remsz);
        rb->wpos = rb->start + len - remsz;
    }

    if (len > 0)
        rb->empty = 0;

    _rb_unlock(rb);
    return len;
}

size_t
ringbuffer_read(ringbuffer_t *rb, void *data, size_t len)
{
    _rb_lock(rb);

    size_t remsz = rb->end - rb->rpos;
    size_t readable_size = _ringbuffer_readable_size(rb);

    if (len > readable_size)
        len = readable_size;

    if (len <= remsz) {
        memcpy(data, rb->rpos, len);
        rb->rpos += len;
    } else {
        memcpy(data, rb->rpos, remsz);
        memcpy((char *)data + remsz, rb->start, len - remsz);
        rb->rpos = rb->start + len - remsz;
    }

    if (rb->rpos == rb->wpos)
        rb->empty = 1;

    _rb_unlock(rb);
    return len;
}

size_t
ringbuffer_peek(ringbuffer_t *rb, void *data, size_t len)
{
    _rb_lock(rb);

    size_t remsz = rb->end - rb->rpos;
    size_t readable_size = _ringbuffer_readable_size(rb);

    if (len > readable_size)
        len = readable_size;

    if (len <= remsz) {
        memcpy(data, rb->rpos, len);
    } else {
        memcpy(data, rb->rpos, remsz);
        memcpy((char *)data + remsz, rb->start, len - remsz);
    }

    _rb_unlock(rb);
    return len;
}

size_t
ringbuffer_drop(ringbuffer_t *rb, size_t len)
{
    _rb_lock(rb);

    size_t remsz = rb->end - rb->rpos;
    size_t readable_size = _ringbuffer_readable_size(rb);

    if (len > readable_size)
        len = readable_size;

    if (len <= remsz) {
        rb->rpos += len;
    } else {
        rb->rpos = rb->start + len - remsz;
    }

    if (rb->rpos == rb->wpos)
        rb->empty = 1;

    _rb_unlock(rb);
    return len;
}
