#include "packet_definition.h"

#include "utils/protocol_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WRITE_CONST_STRING(string, dst)  \
    {                                    \
        const char str[] = (string);     \
        memcpy((dst), str, sizeof(str)); \
        dst += sizeof(str);              \
    }

#define WRITE_STRING(string, dst)          \
    {                                      \
        size_t len = strlen((string)) + 1; \
        memcpy((dst), (string), len);      \
        (dst) += len;                      \
    }

void write_status_response(char *const          dst,
                           const StatusResponse value,
                           uint *const          out_written_bytes) {
    char *buffer = calloc(1000, sizeof(char));
    char *writer = buffer;

    WRITE_CONST_STRING("{\"version\":{\"name\":\"", writer);
    WRITE_STRING(value.version_name, writer);

    WRITE_CONST_STRING("\",\"protocol\":", writer);
    writer += sprintf(writer, "%d", value.version_protocol);

    WRITE_CONST_STRING("},\"players\":{\"max\":", writer);
    writer += sprintf(writer, "%d", value.max_players);

    WRITE_CONST_STRING(",\"online\":0,\"sample\":[]},\"description\":{\"text\":\"", writer);
    WRITE_STRING(value.motd, writer);

    WRITE_CONST_STRING("\"},\"ensuresSecureChat\":true}", writer);

    write_prefixed_bytes(dst, buffer, strlen(buffer), out_written_bytes);
    free(buffer);
}

long read_ping_request(const char *const src, uint *const out_consumed_bytes) {
    *out_consumed_bytes = sizeof(long);
    return *((const long *)src);
}

void write_pong_response(char *const dst, const long timestamp, uint *const out_written_bytes) {
    *out_written_bytes  = sizeof(long);
    *((long *const)dst) = timestamp;
}
