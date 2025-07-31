#include "packets/c2s/handshaking.h"

#include "utils/protocol_utils.h"
#include "var_int.h"

#include <stdint.h>
#include <stdlib.h>

uint32_t read_handshake_packet(HandshakePacket *const dst, const char *const src) {
    uint32_t length;

    int      protocol;
    char    *server_address;
    uint16_t server_port;
    int      intent;

    length = read_var_int(&protocol, src);
    length += read_prefixed_string(&server_address, src + length);

    server_port = *(const uint16_t *)(src + length);
    length += sizeof(short);

    length += read_var_int(&intent, src + length);

    *dst = (HandshakePacket){.protocol_version = protocol,
                             .server_address   = server_address,
                             .server_port      = server_port,
                             .intent           = intent};

    return length;
}

void free_HandshakePacket(HandshakePacket packet) {
    free(packet.server_address);
}

