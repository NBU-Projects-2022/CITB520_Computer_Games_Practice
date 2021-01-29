#pragma once

#include <glm/glm.hpp>
#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "utils.h"

struct PositionComponent {
    ECS_DECLARE_TYPE;

    PositionComponent(float x, float y) : x(x), y(y) {}
    PositionComponent() : x(0.f), y(0.f) {}

    float x;
    float y;
};

ECS_DEFINE_TYPE(PositionComponent);

struct RotationComponent {
    ECS_DECLARE_TYPE;

    RotationComponent(float angle) : angle(angle) {}
    RotationComponent() : angle(0) {}
    float angle;
};

ECS_DEFINE_TYPE(RotationComponent);

class Sprite;
struct RenderComponent {
    ECS_DECLARE_TYPE;

    RenderComponent(Ref<Sprite> sprite) : sprite(sprite) {}

    Ref<Sprite> sprite;
};

ECS_DEFINE_TYPE(RenderComponent);

class Collider;
struct ColliderComponent {
    ECS_DECLARE_TYPE;

    ColliderComponent(Ref<Collider> collider)
        : collider(collider)
    {}

    Ref<Collider> collider;
};

ECS_DEFINE_TYPE(ColliderComponent);

struct RigidBodyComponent {
    ECS_DECLARE_TYPE;

    RigidBodyComponent(bool isStatic = false, bool isKinematic = false)
        : isStatic(isStatic), isKinematic(isKinematic),
        velocity(0.0f), acceleration(0.0f)
    {}

    bool isStatic;
    bool isKinematic;

    glm::vec2 velocity;
    glm::vec2 acceleration;
};

ECS_DEFINE_TYPE(RigidBodyComponent);

class BehaviourScript;
struct BehaviourComponent {
    ECS_DECLARE_TYPE;

    BehaviourComponent(Ref<BehaviourScript> behaviourScript)
        : behaviourScript(behaviourScript)
    {}

    Ref<BehaviourScript> behaviourScript;
};

ECS_DEFINE_TYPE(BehaviourComponent);
