#include "Scripts.h"

#include "ComponentSystem/Components.h"

void ShovelControllerScript::OnInit() {
    AddComponent<TransformComponent>();
    Collider* mouseCollider = new BoxCollider(GetEntity(), 0, 0, 1, 1);
    mouseCollider->collidesWithLayers = CollisionLayers::LAYER_MASK | CollisionLayers::PLANT;
    AddComponent<ColliderComponent>(Ref<Collider>(mouseCollider));
}

void ShovelControllerScript::Update(float deltaTime) {
    ImGuiIO& io = ImGui::GetIO();
    auto& instance = GameState::Instance();
    auto & transform = GetComponent<TransformComponent>();
    transform.position.x = io.MousePos.x;
    transform.position.y = SCREEN_HEIGHT - io.MousePos.y;

    if (io.MousePos.x < 0 || io.MousePos.x > SCREEN_WIDTH
        || io.MousePos.y < 0 || io.MousePos.y > SCREEN_HEIGHT
        || !instance.usingShovel
        || !io.MouseClicked[0]
    ) {
        return;
    }

    auto & collider = GetComponent<ColliderComponent>();
    for (auto & collision : collider.collider->collisions) {
        auto * plantScript = collision.otherEntity->get<NativeScriptComponent>()->Script<PlantScript>();
        if (plantScript) {
            instance.money += plantScript->GetPrice() / 2;
            plantScript->shouldDestroy = true;
            instance.usingShovel = false;
            usedShovel = true;
            break;
        }
    }

    if (usedShovel) {
        instance.usingShovel = false;
        usedShovel = false;
    }
}
