#include <iostream>

#pragma warning(push, 0)
#include <SDL.h>
#pragma warning(pop)

#include "Scripts.h"
#include "ComponentSystem/Components.h"
#include "Core/AssetManager.h"
#include "Physics/Colliders.h"
#include "../Game.h"



void PlantScript::OnInit()
{
    placed = false;
    spriteWidth = GetComponent<RenderComponent>().sprite->width;
    spriteHeight = GetComponent<RenderComponent>().sprite->height;
}

void PlantScript::OnDestroy() {
    if (spawner && spawner->has<NativeScriptComponent>()) {
        spawner->get<NativeScriptComponent>()->Script<NativeScript>()->shouldDestroy = true;
    }
}

void PlantScript::Update(float deltaTime)
{
    if (!placed)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto& currentPlant = GameState::Instance().plantStates.GetPlantByType(PlantSpawnScript::plantType);

        if (io.MousePos.x < 0 || io.MousePos.x > SCREEN_WIDTH ||
            io.MousePos.y < 0 || io.MousePos.y > SCREEN_HEIGHT)
        {
            PlantSpawnScript::isHoldingPlant = false;
            shouldDestroy = true;
        }
        else if (io.MouseClicked[0] && GameState::Instance().money > currentPlant.cost)
        {
            PlacePlantLogic();
        }
        else
        {
            x = io.MousePos.x - spriteWidth / 2;
            y = SCREEN_HEIGHT - io.MousePos.y - spriteHeight / 2;
            GetComponent<TransformComponent>().position.x = x;
            GetComponent<TransformComponent>().position.y = y;
        }
    } else if (hp <= 0) {
        shouldDestroy = true;
    }
}

void PlantScript::PlacePlantLogic() {
    bool isOverALandPlot = false;
    bool isNotOverAPlant = true;
    glm::vec3* plotPos = nullptr;
    Collider * plotCollider = nullptr;

    //restrictions for placing plants go here
    auto & collisions = GetComponent<ColliderComponent>().collider->collisions;
    for (auto& collision : collisions)
    {
        auto & collider = collision.otherEntity->get<ColliderComponent>()->collider;
        if (int(collider->collisionLayer & CollisionLayers::GROUND) > 0) {
            isOverALandPlot = true;
            plotPos = &collision.otherEntity->get<TransformComponent>()->position;
            plotCollider = collider.get();
        }

        if (int(collider->collisionLayer & CollisionLayers::PLANT) > 0) {
            isNotOverAPlant = false;
        }
    }

    if (isOverALandPlot && isNotOverAPlant)
    {
        PlantSpawnScript::isHoldingPlant = false;

        // setup actual collision box size
        auto* collider = reinterpret_cast<BoxCollider*>(GetComponent<ColliderComponent>().collider.get());
        collider->_min[0] = 0;
        collider->_min[1] = 0;
        collider->_max[0] = PLOT_W;
        collider->_max[1] = PLOT_H;
        collider->debugColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        collider->collisionLayer = (collider->collisionLayer ^ CollisionLayers::LAYER_MASK) | (plotCollider->collisionLayer & CollisionLayers::LAYER_MASK) | CollisionLayers::PLANT;
        collider->collidesWithLayers |= CollisionLayers::ZOMBIE;

        // snap to land plot
        GetComponent<TransformComponent>().position.x = plotPos->x;
        GetComponent<TransformComponent>().position.y = plotPos->y;
        GetComponent<TransformComponent>().position.z = plotPos->z + DEBUG_DRAW_GROUND_OFFSET;

        placed = true;
        auto& position = GetComponent<TransformComponent>().position;

        auto& currentPlant = GameState::Instance().plantStates.GetPlantByType(PlantSpawnScript::plantType);
        GameState::Instance().money -= currentPlant.cost;
        hp = currentPlant.hp;

        auto physicsLayer = (collider->collisionLayer & CollisionLayers::LAYER_MASK);
        if (currentPlant.isShooter)
        {
            spawner = CreateGameObject();
            spawner->assign<TransformComponent>(position.x + spriteWidth, position.y + spriteHeight / 2, DRAW_LAYER_10);
            auto * spawnScript = spawner->assign<NativeScriptComponent>()->Bind<BulletSpawnScript>();
            spawnScript->SetCollisionLayer(physicsLayer);
        }

        if (currentPlant.isSpawningSuns)
        {
            spawner = CreateGameObject();
            spawner->assign<TransformComponent>(position.x, position.y + spriteHeight / 2, DRAW_LAYER_10);
            auto * spawnScript = spawner->assign<NativeScriptComponent>()->Bind<SunSpawnScript>();
            spawnScript->SetCollisionLayer(physicsLayer);
        }
    }
}

void PlantScript::FixCollider(Collider * collider) {
    // MOVED FROM PlantSpawnScript
    // talk to Boyan don't know if we need it

    // Here we offsetthe sprite around the collision box.
	// We compare the sizes of the collision box (which has the same size as a single land plot) and the sprite.
	// Whichever is smaller gets centered within the bigger, with the exception where
	// if the sprite is taller than the collision height, then we want to keep the collision at the bottom of the sprite,
	// rather than centering it vertically.

	// bool plotWidthIsLarger = false;
	// bool plotHeightIsLarger = false;
	// float xOffset = 0.0f;
	// float yOffset = 0.0f;

	// if (PLOT_W > w) {
	// 	// center sprite within collision horizontally
	// 	plotWidthIsLarger = true;
	// 	xOffset = (PLOT_W - w) / 2;
	// }
	// else if (w > PLOT_W) {
	// 	// center collision within sprite horizontally
	// 	xOffset = (w - PLOT_W) / 2;
	// }
	// if (PLOT_H > h) {
	// 	// center sprite within collision vertically
	// 	plotHeightIsLarger = true;
	// 	yOffset = (PLOT_H - h) / 2;
	// }
	// else if (h > PLOT_H) {
	// 	// send collision to the bottom of the sprite
	// 	yOffset = h - PLOT_H;
	// }

	// // offset is inverted if a plot dimension is smaller than the respective sprite dimension
	// if (!plotWidthIsLarger && xOffset != 0.0f) xOffset *= -1;
	// if (!plotHeightIsLarger && yOffset != 0.0f) yOffset *= -1;
}
