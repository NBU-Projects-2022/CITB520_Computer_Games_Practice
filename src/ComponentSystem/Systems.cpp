#include "Systems.h"
#include "Physics/Colliders.h"
#include "Game/Scripts.h"

void BehaviourSystem::tick(ECS::World* world, float deltaTime) {
    world->each<BehaviourComponent>(
        [&](ECS::Entity* ent,
            ECS::ComponentHandle<BehaviourComponent> behaviour
        ) {
            behaviour->behaviourScript.get()->Update(ent, deltaTime);
        });
}

void PhysicsSystem::tick(ECS::World* world, float deltaTime) {
    const auto& it = world->each<PositionComponent, RotationComponent, ColliderComponent>();
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

            if (entityCollider->collider->CheckForCollisionWith(otherCollider->collider.get())) {
                entityCollider->collider->collisions.push_back(Collider::Collision { otherEntity });
                otherCollider->collider->collisions.push_back(Collider::Collision { entity });
            }
        }
    }

    world->each<PositionComponent, RotationComponent, ColliderComponent, RigidBodyComponent>(
        [&](ECS::Entity* ent,
            ECS::ComponentHandle<PositionComponent> pos,
            ECS::ComponentHandle<RotationComponent> rot,
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

            pos->y += rigidBody->velocity.y * deltaTime;
            if (pos->y < 120) {
                pos->y = 120;
                rigidBody->acceleration = glm::vec2(0.0f);
                rigidBody->velocity = glm::vec2(0.0f);
            }

            // rot->angle += deltaTime * 2;
        });
}
