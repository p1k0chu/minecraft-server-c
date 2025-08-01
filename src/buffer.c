#include "buffer.h"

#include "utils/utils.h"
#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

inline void buffer_reader_increment(BufferReader *const this, const size_t n) {
    this->ptr += n;
    this->remaining -= n;
}

BufferWriter new_buffer_writer(size_t initial_capacity) {
    char *ptr = malloc(initial_capacity);

    if (ptr == NULL && initial_capacity > 0) {
        error("failed to allocate a buffer");
    }

    return (BufferWriter){.start = ptr, .ptr = ptr, .capacity = initial_capacity};
}

bool buffer_writer_ensure_can_write(BufferWriter *this, size_t n) {
    const size_t written   = this->ptr - this->start;
    const size_t remaining = this->capacity - written;

    if (remaining >= n) return true;

    const size_t capacity = written + n + BUFFER_REALLOC_EXTRA_BYTES;
    char *const  ptr      = realloc(this->start, capacity);

    if (ptr == NULL) error("error resizing buffer");

    this->ptr      = ptr + written;
    this->start    = ptr;
    this->capacity = capacity;

    return true;
}

bool write_prefixed_bytes(BufferWriter *dst, BufferReader *src, size_t n) {
    if (src->remaining < n) return false;
    if (!write_var_int(dst, n)) return false;
    if (!buffer_writer_ensure_can_write(dst, n)) return false;

    memcpy(dst->ptr, src->ptr, n);

    dst->ptr += n;
    buffer_reader_increment(src, n);

    return true;
}

bool read_prefixed_bytes(char **out_data, size_t *out_size, BufferReader *src) {
    int length;
    if (!read_var_int(&length, src)) return false;
    if (src->remaining < (size_t)length) return false;

    *out_data = malloc(length);
    if (*out_data == NULL) {
        perror("read_prefixed_bytes: error allocating");
        return false;
    }

    memcpy(*out_data, src->ptr, length);
    *out_size = length;

    buffer_reader_increment(src, length);

    return true;
}

bool read_prefixed_string(char **dst, BufferReader *src) {
    int length;
    if (!read_var_int(&length, src)) return false;
    if (src->remaining < (size_t)length) return false;

    *dst = malloc(length + 1);
    if (*dst == NULL) return false;

    memcpy(*dst, src->ptr, length);
    (*dst)[length] = 0;

    buffer_reader_increment(src, length);

    return true;
}

