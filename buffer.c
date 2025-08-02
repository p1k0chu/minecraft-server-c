#include "buffer.h"

#include "utils.h"
#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

inline void buffer_reader_increment(BufferReader *const this, const size_t n) {
    this->ptr += n;
    this->remaining -= n;
}

BufferWriter new_buffer_writer(const size_t initial_capacity) {
    if (initial_capacity <= 0) return (BufferWriter){};

    char *ptr = malloc(initial_capacity);

    if (ptr == NULL) {
        ERROR("failed to allocate a buffer");
    }

    return (BufferWriter){.start = ptr, .ptr = ptr, .capacity = initial_capacity};
}

int send_buffer_writer(const int socket, const BufferWriter *const src) {
    const uint32_t length = src->ptr - src->start;
    send_var_int(socket, length);
    return send(socket, src->start, length, 0);
}

bool buffer_writer_ensure_can_write(BufferWriter *const this, const size_t n) {
    const size_t written   = this->ptr - this->start;
    const size_t remaining = this->capacity - written;

    if (remaining >= n) return true;

    const size_t capacity = written + n + BUFFER_REALLOC_EXTRA_BYTES;
    char *const  ptr      = realloc(this->start, capacity);

    if (ptr == NULL) ERROR("error resizing buffer");

    this->ptr      = ptr + written;
    this->start    = ptr;
    this->capacity = capacity;

    return true;
}

bool write_bytes_from_reader(BufferWriter *const dst, BufferReader *const src, const size_t n) {
    if (src->remaining < n) return false;
    if (!write_var_int(dst, n)) return false;

    if (!write_prefixed_bytes(dst, src->ptr, n)) return false;

    buffer_reader_increment(src, n);

    return true;
}

bool write_prefixed_bytes(BufferWriter *const dst, const void *const src, const size_t n) {
    if (!buffer_writer_ensure_can_write(dst, n)) return false;

    memcpy(dst->ptr, src, n);
    dst->ptr += n;

    return true;
}

bool read_prefixed_bytes(char **const out_data, size_t *const out_size, BufferReader *const src) {
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

bool read_prefixed_string(char **const dst, BufferReader *const src) {
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

