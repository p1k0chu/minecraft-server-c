#pragma once

#include <sys/types.h>

typedef enum HandshakeIntent {
    STATUS_INTENT   = 1,
    LOGIN_INTENT    = 2,
    TRANSFER_INTENT = 3
} HandshakeIntent;

typedef struct HandshakePacket {
    int             protocol_version;
    char           *server_address;
    unsigned short  server_port;
    HandshakeIntent intent;
} HandshakePacket;

void free_HandshakePacket(HandshakePacket packet);

typedef struct StatusResponse {
    const char *version_name;
    int         version_protocol;
    uint        max_players;
    const char *motd;
} StatusResponse;

void write_status_response(char *dst, StatusResponse value, uint *out_written_bytes);

HandshakePacket read_handshake_packet(const char *src, uint *out_consumed_bytes);
long            read_ping_request(const char *src, uint *out_consumed_bytes);
void            write_pong_response(char *dst, long timestamp, uint *out_written_bytes);

