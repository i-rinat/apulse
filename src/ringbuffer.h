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

#ifndef APULSE__RINGBUFFER_H
#define APULSE__RINGBUFFER_H

#include <pthread.h>

typedef struct {
    char               *start;
    char               *end;
    char               *rpos;
    char               *wpos;
    pthread_mutex_t     lock;
    int                 empty;
} ringbuffer_t;


ringbuffer_t *
ringbuffer_new(size_t sz);

void
ringbuffer_free(ringbuffer_t *rb);

size_t
ringbuffer_readable_size(ringbuffer_t *rb);

size_t
ringbuffer_writable_size(ringbuffer_t *rb);

size_t
ringbuffer_write(ringbuffer_t *rb, const void *data, size_t len);

size_t
ringbuffer_read(ringbuffer_t *rb, void *data, size_t len);

/** get data from ring buffer without advancing read pointer */
size_t
ringbuffer_peek(ringbuffer_t *rb, void *data, size_t len);

size_t
ringbuffer_drop(ringbuffer_t *rb, size_t len);

#endif // APULSE__RINGBUFFER_H
