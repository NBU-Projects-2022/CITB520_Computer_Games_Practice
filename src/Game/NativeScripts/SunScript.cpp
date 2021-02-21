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
}

void SunScript::Collect() {
    GameState::Instance().money += sunValue;
    shouldDestroy = true;
}
