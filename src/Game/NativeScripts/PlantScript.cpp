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

void PlantScript::Update(float deltaTime)
{
    if (!placed)
    {
        io = ImGui::GetIO();

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

            auto bulletSpawn = CreateGameObject();
            bulletSpawn->assign<TransformComponent>(position.x + spriteWidth, position.y + spriteHeight / 2, 0);
            bulletSpawn->assign<NativeScriptComponent>()->Bind<BulletSpawnScript>();
        }
        else
        {
            x = io.MousePos.x - spriteWidth / 2;
            y = SCREEN_HEIGHT - io.MousePos.y - spriteHeight / 2;
            GetComponent<TransformComponent>().position.x = x;
            GetComponent<TransformComponent>().position.y = y;
        }
       
    }
}
