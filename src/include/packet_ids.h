#pragma once

typedef enum ProtocolStage {
    HANDSHAKING,
    STATUS,
    LOGIN,
    CONFIGURATION,
    PLAY
} ProtocolStage;

/*************************
 ** SERVERBOUND PACKETS **
 *************************/

enum C2SHandshakingPacket {
    HANDSHAKE
};

enum C2SStatusPacket {
    STATUS_REQUEST,
    PING_REQUEST
};

/*************************
 ** CLIENTBOUND PACKETS **
 *************************/

enum S2CStatusPacket {
    STATUS_RESPONSE,
    PONG_RESPONSE
};

