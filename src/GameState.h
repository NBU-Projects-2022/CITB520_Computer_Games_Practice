#pragma once

#include <cstdint>

struct GameState {
// public:
    GameState()
        : progress(0.0f)
    {}

    // vehicle, enemies, ...?

// private:
    float progress;
};
