#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void BulletScript::OnInit()
{
    attackDamage = .2f;
}

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
        if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0)
        {
            collision.otherEntity->get<NativeScriptComponent>()->Script<NativeScript>()->hp -= attackDamage;
            shouldDestroy = true;
            break;
        }
    }
}
