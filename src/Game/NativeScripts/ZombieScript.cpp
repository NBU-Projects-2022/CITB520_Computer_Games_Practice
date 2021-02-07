#include "Scripts.h"

#include <imgui.h>
#include "ComponentSystem/Components.h"
#include <iostream>
void ZombieScript::Update(float deltaTime) {
    auto & rigidBody = GetComponent<RigidBodyComponent>();
    rigidBody.velocity.x = -zombieSpeed;

    // ImGuiIO& io = ImGui::GetIO();
    // if (rigidBody.isKinematic) {
    //     rigidBody.isKinematic = false;
    // } else if (ImGui::IsKeyReleased(ImGui::GetKeyIndex(ImGuiKey_Space))) {
    //     glm::vec2 & velocity = rigidBody.velocity;
    //     glm::vec2 & acceleration = rigidBody.acceleration;
    //     if (glm::abs(velocity.y) < 0.1) {
    //         acceleration.y = 0;
    //         velocity.y = 150;
    //         rigidBody.isKinematic = true;
    //     }
    // }
}
