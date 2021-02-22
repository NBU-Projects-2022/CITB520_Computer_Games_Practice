#pragma once

#include <random>

class Random {
public:
    Random() = default;

    static Random & Instance() {
        static Random instance;
        return instance;
    }

    int Int(int minValue, int maxValue) {
        std::uniform_int_distribution<int> uniformDistribution(minValue, maxValue);
        return uniformDistribution(generator);
    }

    float Float(float minValue, float maxValue) {
        std::uniform_real_distribution<> uniformDistribution(minValue, maxValue);
        return uniformDistribution(generator);
    }

private:
    std::default_random_engine generator;
};
