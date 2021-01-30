#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Components.h"

class NativeScriptSystem : public ECS::EntitySystem {
public:
    NativeScriptSystem() {}

    virtual ~NativeScriptSystem() = default;

    virtual void tick(ECS::World* world, float deltaTime) override;
};

class PhysicsSystem : public ECS::EntitySystem {
public:
    PhysicsSystem(float amount)
        : gravityAmount(amount)
    {}

    virtual ~PhysicsSystem() = default;

    virtual void tick(ECS::World* world, float deltaTime) override;

private:
    float gravityAmount;
};
