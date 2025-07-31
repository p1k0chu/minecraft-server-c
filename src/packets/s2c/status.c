#include "packets/s2c/status.h"

#include "utils/protocol_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WRITE_CONST_STRING(string, dst)  \
    {                                    \
        const char str[] = (string);     \
        memcpy((dst), str, sizeof(str)); \
        dst += sizeof(str) - 1;          \
    }

#define WRITE_STRING(string, dst)      \
    {                                  \
        size_t len = strlen((string)); \
        memcpy((dst), (string), len);  \
        (dst) += len;                  \
    }

uint32_t write_status_response(char *dst, StatusResponse value) {
    uint32_t length = 0;
    char    *buffer = calloc(1000, sizeof(char));
    char    *writer = buffer;

    WRITE_CONST_STRING("{\"version\":{\"name\":\"", writer);
    WRITE_STRING(value.version_name, writer);

    WRITE_CONST_STRING("\",\"protocol\":", writer);
    writer += sprintf(writer, "%d", value.version_protocol);

    WRITE_CONST_STRING("},\"players\":{\"max\":", writer);
    writer += sprintf(writer, "%d", value.max_players);

    WRITE_CONST_STRING(",\"online\":0,\"sample\":[]},\"description\":{\"text\":\"", writer);
    WRITE_STRING(value.motd, writer);

    WRITE_CONST_STRING("\"},\"enforcesSecureChat\":false}", writer);

    length = write_prefixed_bytes(dst, buffer, writer - buffer);
    free(buffer);

    return length;
}

uint32_t write_pong_response(char *dst, long timestamp) {
    *((long *const)dst) = timestamp;
    return sizeof(long);
}

