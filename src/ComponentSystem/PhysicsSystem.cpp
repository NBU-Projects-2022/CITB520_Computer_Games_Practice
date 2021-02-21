#include "Systems.h"
#include "Physics/Colliders.h"

void PhysicsSystem::tick(ECS::World* world, float deltaTime) {
    std::vector<ECS::Entity*> objects;
    objects.reserve(objectsCount);
    world->each<TransformComponent, ColliderComponent>(
        [&](ECS::Entity* ent,
            ECS::ComponentHandle<TransformComponent> transform,
            ECS::ComponentHandle<ColliderComponent> collider
        ) {
            collider->collider->collisions.clear();
            objects.push_back(ent);
        });

    for (auto i = 0; i < objects.size(); ++i) {
        auto & entity = objects[i];
        const auto & entityCollider = entity->get<ColliderComponent>();
        for (int j = i + 1; j < objects.size(); ++j) {
            auto & otherEntity = objects[j];
            const auto & otherCollider = otherEntity->get<ColliderComponent>();
            bool entityCanCollide = bool(entityCollider->collider->collidesWithLayers & otherCollider->collider->collisionLayer & CollisionLayers::LOGIC_LAYERS_MASK)
                && bool(entityCollider->collider->collidesWithLayers & otherCollider->collider->collisionLayer & CollisionLayers::LAYER_MASK);
            bool otherEntityCanCollide = bool(otherCollider->collider->collidesWithLayers & entityCollider->collider->collisionLayer & CollisionLayers::LOGIC_LAYERS_MASK)
                && bool(otherCollider->collider->collidesWithLayers & entityCollider->collider->collisionLayer & CollisionLayers::LAYER_MASK);

            if (!entityCanCollide && !otherEntityCanCollide) {
                continue;
            }

            if (!entityCollider->collider->CheckForCollisionWith(otherCollider->collider.get())) {
                continue;
            }

            if (entityCanCollide) {
                entityCollider->collider->collisions.push_back(Collider::Collision { otherEntity });
            }

            if (otherEntityCanCollide) {
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

            transform->position += glm::vec3(rigidBody->velocity * deltaTime, 0);
            if (transform->position.y < 0) {
                transform->position.y = 0;
                rigidBody->acceleration = glm::vec2(0.0f);
                rigidBody->velocity = glm::vec2(0.0f);
            }

            // rot->angle += deltaTime * 2;
        });
}
