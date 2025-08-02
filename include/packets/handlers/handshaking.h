#pragma once

#include "buffer.h"
#include "packets/c2s/handshaking.h"
#include "server.h"

#include <stddef.h>

void handle_handshaking_packet(PlayerConnection    *conn,
                               BufferReader        *packet_reader,
                               C2SHandshakingPacket id);

void handle_handshake(PlayerConnection *conn, BufferReader *packet_reader);

