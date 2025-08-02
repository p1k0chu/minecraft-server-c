#include "packets/c2s/handshaking.h"

#include "buffer.h"
#include "var_int.h"

#include <stdint.h>
#include <stdlib.h>

bool read_handshake_packet(HandshakePacket *const dst, BufferReader *const src) {
    int      protocol;
    char    *server_address;
    uint16_t server_port;
    int      intent;

    if (!read_var_int(&protocol, src)) return false;
    if (!read_prefixed_string(&server_address, src)) return false;

    if (src->remaining < sizeof(short)) return false;

    server_port = *(const uint16_t *)src->ptr;
    buffer_reader_increment(src, sizeof(short));

    if (!read_var_int(&intent, src)) return false;

    *dst = (HandshakePacket){.protocol_version = protocol,
                             .server_address   = server_address,
                             .server_port      = server_port,
                             .intent           = intent};

    return true;
}

inline void free_handshake_packet(const HandshakePacket *const packet) {
    free(packet->server_address);
}

