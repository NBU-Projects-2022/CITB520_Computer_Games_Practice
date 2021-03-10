#include "Scripts.h"

#include "ComponentSystem/Components.h"
#include <iostream>

void GameLostScript::OnInit() {
	AddComponent<TransformComponent>(0, 0, DRAW_LAYER_10);
	Collider* collider = new BoxCollider(GetEntity(), 0.0f, 0.0f, LAWN_X - PLOT_W, SCREEN_HEIGHT);
	collider->collidesWithLayers = CollisionLayers::LAYER_MASK | CollisionLayers::ZOMBIE;
	collider->debugColor = glm::vec4(0.1f, 0.2f, 0.2f, 1.0f);
	AddComponent<ColliderComponent>(Ref<Collider>(collider));
}

void GameLostScript::Update(float deltaTime) {
    auto & gameStateInstance = GameState::Instance();

    for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
    {
        auto& collider = collision.otherEntity->get<ColliderComponent>()->collider;
        if ((int)(collider->collisionLayer & CollisionLayers::ZOMBIE) > 0) {
            gameStateInstance.gameLost = true;
            return;
        }

        std::cout << "should never get here" << std::endl;
    }
}