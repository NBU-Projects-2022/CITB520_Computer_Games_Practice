#include "Systems.h"
#include "Game/NativeScripts/Scripts.h"

void NativeScriptSystem::tick(ECS::World* world, float deltaTime) {
    world->each<NativeScriptComponent>(
        [&](ECS::Entity* ent,
            ECS::ComponentHandle<NativeScriptComponent> nativeScriptComponent
        ) {
            if (!nativeScriptComponent->nativeScript->entity) {
                nativeScriptComponent->nativeScript->entity = ent;
                nativeScriptComponent->nativeScript->OnInit();
            }

            nativeScriptComponent->nativeScript.get()->Update(deltaTime);
        });
}
