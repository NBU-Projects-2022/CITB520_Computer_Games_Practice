#include "Scripts.h"

void WaveControllerScript::OnInit() {
    AddZombies();
}

void WaveControllerScript::Update(float deltaTime) {
    auto & instance = GameState::Instance();
    if (instance.waveCount <= 0) {
        return;
    }

    instance.waveTimer -= deltaTime;
    if (instance.waveTimer < 0.0f) {
        AddWave();
        --instance.waveCount;
        if (instance.waveCount > 0) {
            AddZombies();
        }

        instance.waveTimer += instance.waveInterval;
    } else if (instance.spawnCommands.empty() && instance.spawnedZombies == instance.killedZombies) {
        AddWave();
        --instance.waveCount;
        if (instance.waveCount > 0) {
            AddZombies();
        }

        instance.waveTimer = instance.waveInterval;
    }
}

void WaveControllerScript::AddZombies(float initialZombieTime) {
    auto & instance = GameState::Instance();
    std::vector<float> spawnTimes;
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(rnd.Float(initialZombieTime, instance.waveInterval));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        int zombieType = rnd.Int(0, 1);
        int layerId = rnd.Int(0, ROWS - 1);
        instance.spawnCommands.push({ zombieType, layerId, instance.totalGameTime + spawnTimes[i] });
    }
}

void WaveControllerScript::AddWave() {
    auto & instance = GameState::Instance();
    std::vector<float> spawnTimes;
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(rnd.Float(1.0f, WAVE_DURATION_SPAN));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        int zombieType = rnd.Int(0, 1);
        int layerId = rnd.Int(0, ROWS - 1);
        instance.spawnCommands.push({ zombieType, layerId, instance.totalGameTime + spawnTimes[i] });
    }
}
