#pragma once

#include <glm/glm.hpp>
#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"

enum class ColliderType {
    BOX,
    CIRCLE
};

enum class CollisionLayers : int {
    GROUND = 1 >> 0,
    ZOMBIE = 1 >> 1,
    PLANT = 1 >> 2,
    PROJECTILE = 1 >> 3,
    SUN = 1 >> 4
};
DEFINE_ENUM_OPERATORS(CollisionLayers)

class BoxCollider;
class CircleCollider;

struct Collider {

    virtual ~Collider() = default;

    struct Collision {
        ECS::Entity * otherEntity;
    };

    ECS::Entity * entity;
    CollisionLayers collisionLayer;
    CollisionLayers collidesWithLayers;
    ColliderType type;
    std::vector<Collision> collisions;

    // lazy but should work
    virtual bool CheckForCollisionWith(Collider * collider) = 0;
    virtual bool CheckForCollisionWith(BoxCollider * collider) = 0;
    virtual bool CheckForCollisionWith(CircleCollider * collider) = 0;

protected:
    Collider(ECS::Entity * entity, ColliderType type)
        : entity(entity),
        type(type),
        collisionLayer(CollisionLayers::GROUND),
        collidesWithLayers(CollisionLayers::GROUND)
    {}
};

bool CheckForBoxWithBoxCollision(BoxCollider * b1, BoxCollider * b2);
bool CheckForBoxWithCircleCollision(BoxCollider * b, CircleCollider * c);
bool CheckForCircleWithCircleCollision(CircleCollider * c1, CircleCollider * c2);

struct BoxCollider : public Collider {

    BoxCollider(ECS::Entity * entity, float minX, float minY, float maxX, float maxY)
        : _min(minX, minY), _max(maxX, maxY), Collider(entity, ColliderType::BOX)
    {}

    glm::vec2 _min;
    glm::vec2 _max;

    virtual bool CheckForCollisionWith(Collider * collider) override {
        return collider->CheckForCollisionWith(this);
    }
    virtual bool CheckForCollisionWith(BoxCollider * collider) override {
        return CheckForBoxWithBoxCollision(this, collider);
    }
    virtual bool CheckForCollisionWith(CircleCollider * collider) override {
        return CheckForBoxWithCircleCollision(this, collider);
    }
};

struct CircleCollider : public Collider {

    CircleCollider(ECS::Entity * entity, float radius)
        : radius(radius), Collider(entity, ColliderType::CIRCLE)
    {}

    float radius;

    virtual bool CheckForCollisionWith(Collider * collider) override {
        return collider->CheckForCollisionWith(this);
    }
    virtual bool CheckForCollisionWith(BoxCollider * collider) override {
        return CheckForBoxWithCircleCollision(collider, this);
    }
    virtual bool CheckForCollisionWith(CircleCollider * collider) override {
        return CheckForCircleWithCircleCollision(this, collider);
    }
};
