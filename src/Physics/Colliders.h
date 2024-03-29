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
    NONE = 0,

    LOGIC_LAYERS_MASK = (1 << 15) - 1,
    GROUND = (1 << 0),
    ZOMBIE = (1 << 1),
    PLANT = (1 << 2),
    BULLET = (1 << 3),
    SUN = (1 << 4),
    LAWNMOWER = (1 << 5),
    MOUSE = (1 << 14),

    LAYER_MASK = (63 << 15),
    LAYER_1 = (1 << 15),
    LAYER_2 = (1 << 16),
    LAYER_3 = (1 << 17),
    LAYER_4 = (1 << 18),
    LAYER_5 = (1 << 19),
    LAYER_6 = (1 << 20),
};
DEFINE_ENUM_OPERATORS(CollisionLayers)

class BoxCollider;
class CircleCollider;

class Collider {
public:
    virtual ~Collider() = default;

    struct Collision {
        ECS::Entity * otherEntity;
    };

    ECS::Entity * entity;
    CollisionLayers collisionLayer;
    CollisionLayers collidesWithLayers;
    ColliderType type;
    std::vector<Collision> collisions;
    glm::vec4 debugColor;

    // lazy but should work
    virtual bool CheckForCollisionWith(Collider * collider) = 0;
    virtual bool CheckForCollisionWith(BoxCollider * collider) = 0;
    virtual bool CheckForCollisionWith(CircleCollider * collider) = 0;

protected:
    Collider(ECS::Entity * entity, ColliderType type)
        : entity(entity),
        type(type),
        collisionLayer(CollisionLayers::NONE),
        collidesWithLayers(CollisionLayers::NONE),
        debugColor(1.0f)
    {}
};

bool CheckForBoxWithBoxCollision(BoxCollider * b1, BoxCollider * b2);
bool CheckForBoxWithCircleCollision(BoxCollider * b, CircleCollider * c);
bool CheckForCircleWithCircleCollision(CircleCollider * c1, CircleCollider * c2);

class BoxCollider : public Collider {
public:
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

class CircleCollider : public Collider {
public:
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
