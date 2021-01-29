#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Components.h"

class BehaviourSystem : public ECS::EntitySystem {
public:
    BehaviourSystem() {}

    virtual ~BehaviourSystem() {}

    virtual void tick(ECS::World* world, float deltaTime) override;
};

class PhysicsSystem : public ECS::EntitySystem {
public:
    PhysicsSystem(float amount) {
        gravityAmount = amount;
    }

    virtual ~PhysicsSystem() {}

    virtual void tick(ECS::World* world, float deltaTime) override;

private:
    float gravityAmount;
};
