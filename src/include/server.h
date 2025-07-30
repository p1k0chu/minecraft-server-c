#pragma once

#include "packet_ids.h"

typedef struct PlayerConnection {
    ProtocolStage stage;
    const int socket;
} PlayerConnection;

