#pragma once

#include <cstdint>

#include "Game/Level.h"

struct GameState {
// public:
    GameState()
        : progress(0.0f)
    {}

    // vehicle, enemies, ...?

    Level level;

// private:
    float progress;
};
