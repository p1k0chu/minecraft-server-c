#include "packets/s2c/status.h"

#include "buffer.h"
#include "var_int.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WRITE_CONST_STRING(string, dst)                     \
    {                                                       \
        const char   str[] = (string);                      \
        const size_t len   = sizeof(str) - 1;               \
        if (!buffer_writer_ensure_can_write(&(dst), len)) { \
            return false;                                   \
        }                                                   \
        memcpy((dst).ptr, str, len);                        \
        (dst).ptr += len;                                   \
    }

#define WRITE_STRING(string, dst)                           \
    {                                                       \
        const size_t len = strlen((string));                \
        if (!buffer_writer_ensure_can_write(&(dst), len)) { \
            return false;                                   \
        }                                                   \
        memcpy((dst).ptr, (string), len);                   \
        (dst).ptr += len;                                   \
    }

#define LENGTH_IN_STR(number) snprintf(NULL, 0, "%d", (number))

#define WRITE_NUMBER_AS_STRING(buffer, number)                 \
    {                                                          \
        const size_t len = LENGTH_IN_STR((number));            \
        if (!buffer_writer_ensure_can_write(&(buffer), len)) { \
            return false;                                      \
        }                                                      \
        snprintf((buffer).ptr, len + 1, "%d", (number));       \
        (buffer).ptr += len;                                   \
    }

bool write_status_response(BufferWriter *dst, const StatusResponse *value) {
    if (!write_var_int(dst, STATUS_RESPONSE)) return false;

    BufferWriter buffer = new_buffer_writer(BUFFER_REALLOC_EXTRA_BYTES);
    size_t       len;

    WRITE_CONST_STRING("{\"version\":{\"name\":\"", buffer);
    WRITE_STRING(value->version_name, buffer);

    WRITE_CONST_STRING("\",\"protocol\":", buffer);

    WRITE_NUMBER_AS_STRING(buffer, value->version_protocol);

    WRITE_CONST_STRING("},\"players\":{\"max\":", buffer);
    WRITE_NUMBER_AS_STRING(buffer, value->max_players);

    WRITE_CONST_STRING(",\"online\":0,\"sample\":[]},\"description\":{\"text\":\"", buffer);
    WRITE_STRING(value->motd, buffer);

    WRITE_CONST_STRING("\"},\"enforcesSecureChat\":false}", buffer);

    len = buffer.ptr - buffer.start;

    BufferReader reader = {.ptr = buffer.start, .remaining = len};
    write_prefixed_bytes(dst, &reader, reader.remaining);

    free(buffer.start);

    return true;
}

bool write_pong_response(BufferWriter *dst, long timestamp) {
    if (!write_var_int(dst, PONG_RESPONSE)) return false;

    *(long *)(dst->ptr) = timestamp;
    dst->ptr += sizeof(long);

    return true;
}

