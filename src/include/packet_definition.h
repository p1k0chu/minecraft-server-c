#pragma once

#include <sys/types.h>

typedef enum HandshakeIntent {
    STATUS_INTENT   = 1,
    LOGIN_INTENT    = 2,
    TRANSFER_INTENT = 3
} HandshakeIntent;

typedef struct HandshakePacket {
    const int             protocol_version;
    char *const           server_address;
    const unsigned short  server_port;
    const HandshakeIntent intent;
} HandshakePacket;

void free_HandshakePacket(HandshakePacket packet);

typedef struct StatusResponse {
    const char *const version_name;
    const int         version_protocol;
    const uint        max_players;
    const char *const motd;
} StatusResponse;

void write_status_response(char *dst, StatusResponse value, uint *out_written_bytes);

HandshakePacket read_handshake_packet(const char *src, uint *out_consumed_bytes);
long            read_ping_request(const char *src, uint *out_consumed_bytes);
void            write_pong_response(char *dst, long timestamp, uint *out_written_bytes);

