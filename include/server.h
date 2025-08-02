#pragma once

#include "packets/protocol_stage.h"

typedef struct PlayerConnection {
    ProtocolStage stage;
    const int     socket;
} PlayerConnection;

