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

    // a bunch of --writer because strings end in null char

    WRITE_CONST_STRING("{\"version\":{\"name\":\"", writer);
    --writer;
    WRITE_STRING(value.version_name, writer);
    --writer;

    WRITE_CONST_STRING("\",\"protocol\":", writer);
    --writer;
    writer += sprintf(writer, "%d", value.version_protocol);

    WRITE_CONST_STRING("},\"players\":{\"max\":", writer);
    --writer;
    writer += sprintf(writer, "%d", value.max_players);

    WRITE_CONST_STRING(",\"online\":0,\"sample\":[]},\"description\":{\"text\":\"", writer);
    --writer;
    WRITE_STRING(value.motd, writer);
    --writer;

    WRITE_CONST_STRING("\"},\"enforcesSecureChat\":false}", writer);

    write_prefixed_bytes(dst, buffer, strlen(buffer), out_written_bytes);
    free(buffer);
}

HandshakePacket read_handshake_packet(const char *const src, uint *const out_consumed_bytes) {
    uint tmp;

    int    protocol;
    char  *server_address;
    ushort server_port;
    int    intent;

    protocol = read_var_int(src, out_consumed_bytes);

    server_address = read_prefixed_bytes(src + *out_consumed_bytes, &tmp);
    *out_consumed_bytes += tmp;

    server_port = *(const ushort *)(src + *out_consumed_bytes);
    *out_consumed_bytes += sizeof(short);

    intent = read_var_int(src + *out_consumed_bytes, &tmp);
    *out_consumed_bytes += tmp;

    HandshakePacket packet = {.protocol_version = protocol,
                              .server_address   = server_address,
                              .server_port      = server_port,
                              .intent           = intent};
    return packet;
}

void free_HandshakePacket(HandshakePacket packet) {
    free(packet.server_address);
}

long read_ping_request(const char *const src, uint *const out_consumed_bytes) {
    *out_consumed_bytes = sizeof(long);
    return *((const long *)src);
}

void write_pong_response(char *const dst, const long timestamp, uint *const out_written_bytes) {
    *out_written_bytes  = sizeof(long);
    *((long *const)dst) = timestamp;
}
