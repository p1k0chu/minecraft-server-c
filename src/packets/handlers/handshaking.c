#include "packets/handlers/handshaking.h"

#include "packets/c2s/handshaking.h"

#include <stdio.h>
#include <stdlib.h>

void handle_handshaking_packet(PlayerConnection *const    conn,
                               const char *const          packet_bytes,
                               const size_t               n_bytes,
                               const C2SHandshakingPacket id) {
    switch (id) {
    case HANDSHAKE:
        handle_handshake(conn, packet_bytes, n_bytes);
        break;
    }
}

void handle_handshake(PlayerConnection *const conn, const char *const packet_bytes, const size_t) {
    HandshakePacket packet;
    read_handshake_packet(&packet, packet_bytes);

    switch (packet.intent) {
    case STATUS_INTENT:
        conn->stage = STATUS;
        break;
    case LOGIN_INTENT:
        conn->stage = LOGIN;
        break;
    case TRANSFER_INTENT:
        fprintf(stderr, "Transfer not implemented\n");
        exit(1);
    default:
        fprintf(stderr, "invalid handshake intent: %d\n", packet.intent);
        exit(1);
    }

    free_HandshakePacket(packet);
}

