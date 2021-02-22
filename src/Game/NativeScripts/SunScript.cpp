#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void SunScript::Update(float deltaTime)
{
    auto& rigidBody = GetComponent<RigidBodyComponent>();
    if (!rigidBody.isKinematic) {
        return;
    }

    flyTime -= deltaTime;
    if (flyTime <= 0.0f) {
        rigidBody.isKinematic = false;
    }
}

void SunScript::Collect() {
    GameState::Instance().money += sunValue;
    shouldDestroy = true;
}
