#pragma once

#include <stdint.h>

typedef enum C2SHandshakingPacket { HANDSHAKE } C2SHandshakingPacket;

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

/**
 * reads one HandshakePacket from src into dst and returns amount of bytes read
 */
uint32_t read_handshake_packet(HandshakePacket *dst, const char *src);

