/* 
 *
 * The MIT License
*
* Copyright (c) Andrew Mochalskyi
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
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
 */

#include <assert.h>
#include <stdlib.h>
#include "ringbuf.h"


struct ringbuf {
    uint8_t *buffer;
    size_t head;
    size_t tail;
    size_t max;
    uint8_t full : 1;
};


Ringbuf *ringbuf_init(uint8_t *buffer, size_t size) {

    assert(buffer && size);

    Ringbuf *rbuf = malloc(sizeof(Ringbuf));
    assert(rbuf);

    rbuf->buffer = buffer;
    rbuf->max = size;
    ringbuf_reset(rbuf);

    assert(ringbuf_empty(rbuf));

    return rbuf;
}


void ringbuf_reset(Ringbuf *rbuf) {

    assert(rbuf);

    rbuf->head = 0;
    rbuf->tail = 0;
    rbuf->full = 0;
}


void ringbuf_free(Ringbuf *rbuf) {
    assert(rbuf);
    free(rbuf);
    rbuf = NULL;
}


uint8_t ringbuf_full(Ringbuf *rbuf) {
    assert(rbuf);
    return rbuf->full;
}


uint8_t ringbuf_empty(Ringbuf *rbuf) {
    assert(rbuf);
    return (!rbuf->full && (rbuf->head == rbuf->tail));
}


size_t ringbuf_capacity(Ringbuf *rbuf) {
    assert(rbuf);
    return rbuf->max;
}


size_t ringbuf_size(Ringbuf *rbuf) {

    assert(rbuf);

    size_t size = rbuf->max;

    if (!rbuf->full) {
        if (rbuf->head >= rbuf->tail) {
            size = (rbuf->head - rbuf->tail);
        } else {
            size = (rbuf->max + rbuf->head - rbuf->tail);
        }
    }

    return size;
}


static void advance_pointer(Ringbuf *rbuf) {

    assert(rbuf);

    if (rbuf->full) {
        rbuf->tail = (rbuf->tail + 1) % rbuf->max;
    }

    rbuf->head = (rbuf->head + 1) % rbuf->max;
    rbuf->full = (rbuf->head == rbuf->tail);
}


static void retreat_pointer(Ringbuf *rbuf) {

    assert(rbuf);

    rbuf->full = 0;
    rbuf->tail = (rbuf->tail + 1) % rbuf->max;
}


int8_t ringbuf_push(Ringbuf *rbuf, uint8_t dest) {

    int8_t r = -1;

    assert(rbuf && rbuf->buffer);

    if (!ringbuf_full(rbuf)) {
        rbuf->buffer[rbuf->head] = dest;
        advance_pointer(rbuf);
        r = 0;
    }

    return r;
}


int8_t ringbuf_bulk_push(Ringbuf *rbuf, uint8_t *dest, size_t size) {

    assert(rbuf && dest && rbuf->buffer);

    int8_t r = 0;

    for (uint8_t i = 0; i < size; ++i) {
        r = ringbuf_push(rbuf, dest[i]);
        if (r == -1) break;
    }

    return r;
}


int8_t ringbuf_pop(Ringbuf *rbuf, uint8_t *dest) {

    assert(rbuf && dest && rbuf->buffer);

    int8_t r = -1;

    if (!ringbuf_empty(rbuf)) {
        *dest = rbuf->buffer[rbuf->tail];
        retreat_pointer(rbuf);

        r = 0;
    }

    return r;
}


int8_t ringbuf_bulk_pop(Ringbuf *rbuf, uint8_t *dest, size_t size) {

    assert(rbuf && dest && rbuf->buffer);

    uint8_t *cur = dest;
    int8_t r = 0;

    while (ringbuf_size(rbuf) > 0 && size > 0) {
        r = ringbuf_pop(rbuf, cur++);
        size--;
        if (r == -1) break;
    }

    return r;
}
