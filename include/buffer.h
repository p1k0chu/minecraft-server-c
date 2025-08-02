#pragma once

#include <stddef.h>
#include <stdint.h>

#define BUFFER_REALLOC_EXTRA_BYTES 32

typedef struct BufferReader {
    const char *ptr;
    size_t      remaining;
} BufferReader;

void buffer_reader_increment(BufferReader *this, size_t n);

typedef struct BufferWriter {
    char  *start;
    size_t capacity;

    char *ptr;
} BufferWriter;

BufferWriter new_buffer_writer(size_t initial_capacity);

/// realloc's the buffer and modifies the struct (on success).
/// returns the success bool.
bool buffer_writer_ensure_can_write(BufferWriter *this, size_t n);

/// writes n bytes from buffer reader to the buffer writer
bool write_prefixed_bytes(BufferWriter *dst, BufferReader *src, size_t n);

/// allocates new array into out_data and reads the prefixed bytes into it.
/// puts the length of the array into out_size
bool read_prefixed_bytes(char **out_data, size_t *out_size, BufferReader *src);

/// allocates new string at dst and reads from src into the new string.
/// resulting string IS null terminated.
bool read_prefixed_string(char **dst, BufferReader *src);

