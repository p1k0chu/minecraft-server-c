#pragma once

#include "packets/c2s/handshaking.h"
#include "server.h"

#include <stddef.h>

void handle_handshaking_packet(PlayerConnection    *conn,
                               const char          *packet_bytes,
                               size_t               n_bytes,
                               C2SHandshakingPacket id);

void handle_handshake(PlayerConnection *conn, const char *packet_bytes, size_t n_bytes);

