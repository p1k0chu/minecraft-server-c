#include "packets/c2s/status.h"
#include "server.h"

#include <stddef.h>

void handle_status_packet(PlayerConnection *conn,
                          const char       *packet_bytes,
                          size_t            n_bytes,
                          C2SStatusPacket   id);

void handle_status_request(PlayerConnection *conn, const char *packet_bytes, size_t n_bytes);
void handle_ping_request(PlayerConnection *conn, const char *packet_bytes, size_t n_bytes);

