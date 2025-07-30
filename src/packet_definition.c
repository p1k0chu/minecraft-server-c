#include "packet_definition.h"

#include "utils/protocol_utils.h"
#include "var_int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

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

uint write_status_response(char *dst, StatusResponse value) {
    uint  length = 0;
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

    WRITE_CONST_STRING("\"},\"enforcesSecureChat\":false}", writer);

    length = write_prefixed_bytes(dst, buffer, writer - buffer);
    free(buffer);

    return length;
}

uint read_handshake_packet(HandshakePacket *const dst, const char *const src) {
    uint length;

    int    protocol;
    char  *server_address;
    ushort server_port;
    int    intent;

    length = read_var_int(&protocol, src);
    length += read_prefixed_bytes(&server_address, src + length);

    server_port = *(const ushort *)(src + length);
    length += sizeof(short);

    length += read_var_int(&intent, src + length);

    *dst = (HandshakePacket){.protocol_version = protocol,
                             .server_address   = server_address,
                             .server_port      = server_port,
                             .intent           = intent};

    return length;
}

void free_HandshakePacket(HandshakePacket packet) {
    free(packet.server_address);
}

uint read_ping_request(long *dst, const char *src) {
    *dst = *((const long *)src);
    return sizeof(long);
}

uint write_pong_response(char *dst, long timestamp) {
    *((long *const)dst) = timestamp;
    return sizeof(long);
}
