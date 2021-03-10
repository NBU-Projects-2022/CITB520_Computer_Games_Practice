#include "Scripts.h"

void WaveControllerScript::OnInit() {
    AddZombies();
}

void WaveControllerScript::Update(float deltaTime) {
    auto & gameStateInstance = GameState::Instance();
    if (gameStateInstance.waveCount <= 0) {
        return;
    }

    gameStateInstance.waveTimer -= deltaTime;
    if (gameStateInstance.waveTimer < 0.0f) {
        AddWave();
        --gameStateInstance.waveCount;
        if (gameStateInstance.waveCount > 0) {
            AddZombies();
        }

        gameStateInstance.waveTimer += WAVE_INTERVAL;
    } else if (gameStateInstance.spawnCommands.empty() && gameStateInstance.spawnedZombies == gameStateInstance.killedZombies) {
        AddWave();
        --gameStateInstance.waveCount;
        if (gameStateInstance.waveCount > 0) {
            AddZombies();
        }

        gameStateInstance.waveTimer = WAVE_INTERVAL;
    }
}

void WaveControllerScript::AddZombies(float initialZombieTime) {
    auto & gameStateInstance = GameState::Instance();
    std::vector<float> spawnTimes;
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(initialZombieTime + 0.1f * i + rnd.Float(0.0f, WAVE_INTERVAL));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        int zombieType = rnd.Int(0, 1);
        int layerId = rnd.Int(0, ROWS - 1);
        gameStateInstance.spawnCommands.push({ zombieType, layerId, gameStateInstance.totalGameTime + spawnTimes[i] });
    }
}

void WaveControllerScript::AddWave() {
    auto & gameStateInstance = GameState::Instance();
    std::vector<float> spawnTimes;
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(0.1f * i + rnd.Float(0.0f, WAVE_DURATION_SPAN));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        int zombieType = rnd.Int(0, 1);
        int layerId = rnd.Int(0, ROWS - 1);
        gameStateInstance.spawnCommands.push({ zombieType, layerId, gameStateInstance.totalGameTime + spawnTimes[i] });
    }
}
