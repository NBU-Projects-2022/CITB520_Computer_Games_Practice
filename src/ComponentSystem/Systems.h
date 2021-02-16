#pragma once

// #include <iostream>

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Components.h"

class NativeScriptSystem : public ECS::EntitySystem {
public:
    NativeScriptSystem() {}

    virtual ~NativeScriptSystem() = default;

    virtual void tick(ECS::World* world, float deltaTime) override;
};

class PhysicsSystem : public ECS::EntitySystem,
    public ECS::EventSubscriber<ECS::Events::OnComponentAssigned<ColliderComponent>>,
    public ECS::EventSubscriber<ECS::Events::OnComponentRemoved<ColliderComponent>>
{
public:
    PhysicsSystem(float amount)
        : gravityAmount(amount),
        objectsCount(0)
    {}

    virtual ~PhysicsSystem() = default;

    virtual void tick(ECS::World* world, float deltaTime) override;

    virtual void configure(class ECS::World* world) override
    {
        world->subscribe<ECS::Events::OnComponentAssigned<ColliderComponent>>(this);
        world->subscribe<ECS::Events::OnComponentRemoved<ColliderComponent>>(this);
    }

    virtual void unconfigure(class ECS::World* world) override
    {
        world->unsubscribeAll(this);
    }

    virtual void receive(class ECS::World* world, const ECS::Events::OnComponentAssigned<ColliderComponent>& event) override {
        // std::cout << "An entity component was assigned!" << std::endl;
        ++objectsCount;
    }

    virtual void receive(class ECS::World* world, const ECS::Events::OnComponentRemoved<ColliderComponent>& event) override {
        // std::cout << "An entity component was removed!" << std::endl;
        --objectsCount;
    }

private:
    float gravityAmount;
    int objectsCount;
};
