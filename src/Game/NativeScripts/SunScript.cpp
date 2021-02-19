#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void SunScript::Update(float deltaTime)
{
    auto& rigidBody = GetComponent<RigidBodyComponent>();
    rigidBody.velocity.y = sunFloatSpeed;

    sunFloatCurrDistance += sunFloatSpeed * deltaTime;

    if (sunFloatCurrDistance >= sunFloatDistance)
    {
        shouldDestroy = true;
    }

    auto& position = GetComponent<TransformComponent>().position;

    if (position.x < 0 || position.x > SCREEN_WIDTH ||
        position.y < 0 || position.y > SCREEN_HEIGHT)
    {
        shouldDestroy = true;
    }
}
