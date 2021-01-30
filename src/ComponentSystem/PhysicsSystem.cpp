#include "Systems.h"
#include "Physics/Colliders.h"

void PhysicsSystem::tick(ECS::World* world, float deltaTime) {
    const auto& it = world->each<TransformComponent, ColliderComponent>();
    for (auto i = it.begin(); i != it.end(); ++i) {
        auto entity = i.get();
        auto entityCollider = entity->get<ColliderComponent>();
        entityCollider->collider->collisions.clear();
    }

    for (auto i = it.begin(); i != it.end(); ++i) {
        auto entity = i.get();
        // auto entityPos = entity->get<PositionComponent>();
        // auto entityRot = entity->get<RotationComponent>();
        auto entityCollider = entity->get<ColliderComponent>();
        auto other = i;
        ++other;
        for (; other != it.end(); ++other) {
            auto otherEntity = other.get();
            // auto pos = entity->get<PositionComponent>();
            // auto rot = entity->get<RotationComponent>();
            auto otherCollider = otherEntity->get<ColliderComponent>();

            if (0 == int(entityCollider->collider->collidesWithLayers & otherCollider->collider->collisionLayer)
                && 0 == int(otherCollider->collider->collidesWithLayers & entityCollider->collider->collisionLayer)
            ) {
                continue;
            }

            if (!entityCollider->collider->CheckForCollisionWith(otherCollider->collider.get())) {
                continue;
            }

            if (bool(entityCollider->collider->collidesWithLayers & otherCollider->collider->collisionLayer)) {
                entityCollider->collider->collisions.push_back(Collider::Collision { otherEntity });
            }

            if (bool(otherCollider->collider->collidesWithLayers & entityCollider->collider->collisionLayer)) {
                otherCollider->collider->collisions.push_back(Collider::Collision { entity });
            }
        }
    }

    world->each<TransformComponent, ColliderComponent, RigidBodyComponent>(
        [&](ECS::Entity* ent,
            ECS::ComponentHandle<TransformComponent> transform,
            ECS::ComponentHandle<ColliderComponent> collider,
            ECS::ComponentHandle<RigidBodyComponent> rigidBody
        ) {
            if (rigidBody->isStatic) {
                return;
            }

            if (!rigidBody->isKinematic) {
                if (collider->collider->collisions.empty()) {
                    rigidBody->acceleration = glm::vec2(0.0f, gravityAmount);
                    rigidBody->velocity += rigidBody->acceleration * deltaTime;
                } else {
                    rigidBody->velocity = glm::vec2(0.0f, 0.0f);
                    rigidBody->acceleration = glm::vec2(0.0f, 0.0f);
                }
            }

            transform->position += rigidBody->velocity * deltaTime;
            if (transform->position.y < 0) {
                transform->position.y = 0;
                rigidBody->acceleration = glm::vec2(0.0f);
                rigidBody->velocity = glm::vec2(0.0f);
            }

            // rot->angle += deltaTime * 2;
        });
}
