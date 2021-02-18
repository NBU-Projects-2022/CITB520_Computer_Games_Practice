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

    

    health = 3;
}

void PlantScript::Update(float deltaTime)
{
    if (!placed)
    {
        ImGuiIO& io = ImGui::GetIO();

        if (io.MousePos.x < 0 || io.MousePos.x > SCREEN_WIDTH ||
            io.MousePos.y < 0 || io.MousePos.y > SCREEN_HEIGHT)
        {
            shouldDestroy = true;
        }
        else if (io.MouseClicked[0])
        {
            //restrictions for placing plants go here
            placed = true;

            auto& position = GetComponent<TransformComponent>().position;
            
            switch (PlantSpawnScript::plantType)
            {
                case (int)PlantTypes::Peashooter:
                    auto bulletSpawn = CreateGameObject();
                    bulletSpawn->assign<TransformComponent>(position.x + spriteWidth, position.y + spriteHeight / 2, DRAW_LAYER_10);
                    bulletSpawn->assign<NativeScriptComponent>()->Bind<BulletSpawnScript>();
                    break;
                case (int)PlantTypes::Sunflower:
                    auto sunSpawn = CreateGameObject();
                    sunSpawn->assign<TransformComponent>(position.x + spriteWidth, position.y + spriteHeight / 2, DRAW_LAYER_10);
                    sunSpawn->assign<NativeScriptComponent>()->Bind<SunSpawnScript>();
                    break;
            }

        }
        else
        {
            x = io.MousePos.x - spriteWidth / 2;
            y = SCREEN_HEIGHT - io.MousePos.y - spriteHeight / 2;
            GetComponent<TransformComponent>().position.x = x;
            GetComponent<TransformComponent>().position.y = y;
        }

    }
    else
    {
        bool collidesWithZombie = false;
        for (auto& collision : GetComponent<ColliderComponent>().collider->collisions)
        {
            //check if this works
            if ((int)(collision.otherEntity->get<ColliderComponent>()->collider->collisionLayer & CollisionLayers::ZOMBIE) > 0)
            {
                collidesWithZombie = true;

                nextDamageIn -= deltaTime;

                if (nextDamageIn <= .0f)
                {
                    nextDamageIn = damageInterval;
                    health--;

                    if (health == 0)
                    {
                        shouldDestroy = true;
                    }
                }
            }
        }

        if (!collidesWithZombie)
        {
            nextDamageIn = damageInterval;
        }
             
    }


}
