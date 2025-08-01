#include "packets/handlers/handshaking.h"

#include "packets/c2s/handshaking.h"

#include <stdio.h>
#include <stdlib.h>

void handle_handshaking_packet(PlayerConnection *const    conn,
                               BufferReader *const        packet,
                               const C2SHandshakingPacket id) {
    switch (id) {
    case HANDSHAKE:
        handle_handshake(conn, packet);
        break;
    }
}

void handle_handshake(PlayerConnection *const conn, BufferReader *const packet_reader) {
    HandshakePacket packet;
    if (!read_handshake_packet(&packet, packet_reader)) return;

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

    free_handshake_packet(&packet);
}

