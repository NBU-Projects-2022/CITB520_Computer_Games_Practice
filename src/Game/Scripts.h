#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

class BehaviourScript {
public:
    virtual void Update(ECS::Entity * entity, float deltaTime) = 0;
};

class VehicleBehaviourScript : public BehaviourScript {
public:
    virtual void Update(ECS::Entity * entity, float deltaTime) override;
};

class EnemyBehaviourScript : public BehaviourScript {
public:
    virtual void Update(ECS::Entity * entity, float deltaTime) override;
};
