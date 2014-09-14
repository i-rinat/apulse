#undef NDEBUG
#include <assert.h>
#include <src/ringbuffer.c>
#include <stdio.h>

int
main(void)
{
    ringbuffer_t *rb = ringbuffer_new(16);
    char buf[20];

    // check initial state
    assert(rb);
    assert(ringbuffer_readable_size(rb) == 0);
    assert(ringbuffer_writable_size(rb) == 16);

    // check for reading from empty buffer
    assert(ringbuffer_read(rb, buf, sizeof(buf)) == 0);

    // write some data
    assert(ringbuffer_write(rb, "hello", 5) == 5);
    assert(ringbuffer_readable_size(rb) == 5);
    assert(ringbuffer_writable_size(rb) == 11);

    // read that data
    assert(ringbuffer_read(rb, buf, sizeof(buf)) == 5);
    assert(memcmp(buf, "hello", 5) == 0);

    // test wrapping
    assert(ringbuffer_write(rb, "0123456789abcdef", 16) == 16);
    assert(ringbuffer_readable_size(rb) == 16);
    assert(ringbuffer_writable_size(rb) == 0);

    // reading of wrapped data in two chunks
    assert(ringbuffer_read(rb, buf, 15) == 15);
    assert(memcmp(buf, "0123456789abcde", 15) == 0);
    assert(ringbuffer_read(rb, buf, 15) == 1);
    assert(memcmp(buf, "f", 1) == 0);

    // check readable/writable size functions in case of buffer wrapping
    assert(ringbuffer_write(rb, "0123456789abcdef", 16) == 16);
    assert(ringbuffer_readable_size(rb) == 16);
    assert(ringbuffer_writable_size(rb) == 0);

    assert(ringbuffer_read(rb, buf, 3) == 3);
    assert(ringbuffer_readable_size(rb) == 13);
    assert(ringbuffer_writable_size(rb) == 3);

    assert(ringbuffer_read(rb, buf, 3) == 3);
    assert(ringbuffer_readable_size(rb) == 10);
    assert(ringbuffer_writable_size(rb) == 6);

    assert(ringbuffer_read(rb, buf, 3) == 3);
    assert(ringbuffer_readable_size(rb) == 7);
    assert(ringbuffer_writable_size(rb) == 9);

    assert(ringbuffer_read(rb, buf, 3) == 3);
    assert(ringbuffer_readable_size(rb) == 4);
    assert(ringbuffer_writable_size(rb) == 12);

    assert(ringbuffer_read(rb, buf, 3) == 3);
    assert(ringbuffer_readable_size(rb) == 1);
    assert(ringbuffer_writable_size(rb) == 15);

    assert(ringbuffer_read(rb, buf, 3) == 1);
    assert(ringbuffer_readable_size(rb) == 0);
    assert(ringbuffer_writable_size(rb) == 16);

    // try to write more than buffer have
    assert(ringbuffer_write(rb, "01234567890", 10) == 10);
    assert(ringbuffer_writable_size(rb) == 6);
    assert(ringbuffer_write(rb, "01234567890", 10) == 6);
    assert(ringbuffer_writable_size(rb) == 0);
    assert(ringbuffer_readable_size(rb) == 16);

    // cleanup
    ringbuffer_free(rb);

    printf("pass\n");
    return 0;
}
