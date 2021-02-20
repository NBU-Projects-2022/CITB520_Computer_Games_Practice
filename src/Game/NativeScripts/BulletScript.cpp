#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void BulletScript::Update(float deltaTime)
{
    auto& rigidBody = GetComponent<RigidBodyComponent>();
    rigidBody.velocity.x = bulletSpeed;

    auto& position = GetComponent<TransformComponent>().position;

    if (position.x < 0 || position.x > SCREEN_WIDTH ||
        position.y < 0 || position.y > SCREEN_HEIGHT)
    {
        shouldDestroy = true;
    }

    for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
    {
        //check if this works
        if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0) 
        {
            shouldDestroy = true;
        }
    }

}
