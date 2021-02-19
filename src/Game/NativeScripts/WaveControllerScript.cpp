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
    std::uniform_int_distribution<int> layerDistribution(0, ROWS - 1);
    std::uniform_int_distribution<int> zombieDistribution(0, 1);
    std::uniform_real_distribution<> timeDistribution(initialZombieTime, instance.waveInterval);
    std::vector<float> spawnTimes;
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(timeDistribution(generator));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < ZOMBIES_COUNT; ++i) {
        int zombieType = zombieDistribution(generator);
        int layerId = layerDistribution(generator);
        instance.spawnCommands.push({ zombieType, layerId, instance.totalGameTime + spawnTimes[i] });
    }
}

void WaveControllerScript::AddWave() {
    auto & instance = GameState::Instance();
    std::uniform_int_distribution<int> layerDistribution(0, ROWS - 1);
    std::uniform_int_distribution<int> zombieDistribution(0, 1);
    std::uniform_real_distribution<> timeDistribution(1.0f, WAVE_DURATION_SPAN);
    std::vector<float> spawnTimes;
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        spawnTimes.push_back(timeDistribution(generator));
    }

    std::sort(spawnTimes.begin(), spawnTimes.end(), std::less<>());
    for (int i = 0; i < WAVE_ZOMBIES_COUNT; ++i) {
        int zombieType = zombieDistribution(generator);
        int layerId = layerDistribution(generator);
        instance.spawnCommands.push({ zombieType, layerId, instance.totalGameTime + spawnTimes[i] });
    }
}
