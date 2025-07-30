#pragma once

typedef enum ProtocolStage { HANDSHAKING, STATUS, LOGIN, CONFIGURATION, PLAY } ProtocolStage;

/*************************
 ** SERVERBOUND PACKETS **
 *************************/

typedef enum C2SHandshakingPacket { HANDSHAKE } C2SHandshakingPacket;

typedef enum C2SStatusPacket { STATUS_REQUEST, PING_REQUEST } C2SStatusPacket;

/*************************
 ** CLIENTBOUND PACKETS **
 *************************/

typedef enum S2CStatusPacket { STATUS_RESPONSE, PONG_RESPONSE } S2CStatusPacket;

