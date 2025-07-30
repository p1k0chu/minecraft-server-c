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

/**
 * returns the amount of bytes written
 */
uint write_status_response(char *dst, StatusResponse value);

/**
 * reads one HandshakePacket from src into dst and returns amount of bytes read
 */
uint read_handshake_packet(HandshakePacket *dst, const char *src);

/**
 * reads one ping request from src into dst and returns amount of bytes read
 *
 * the whole ping request is just one long (timestamp)
 */
uint read_ping_request(long *dst, const char *src);

/**
 * returns the amount of bytes written
 */
uint write_pong_response(char *dst, long timestamp);

