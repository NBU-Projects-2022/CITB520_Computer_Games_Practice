#include "Scripts.h"

#include "ComponentSystem/Components.h"

void GlobalSunSpawnScript::OnInit() {
    sun = AssetManager::LoadCachedImageFile("assets/sprites/Sun.png");
    glm::vec2 size(sun->GetWidth(), sun->GetHeight());
    sunSprite = Sprite(0, 0, size.x, size.y, sun.get(), -size / 2.0f);
}

void GlobalSunSpawnScript::Update(float deltaTime) {
    nextSpawnIn -= deltaTime;
    if (nextSpawnIn < 0) {
        auto sunEntity = CreateGameObject();

        int layerId = rnd.Int(0, 4);
        sunEntity->assign<TransformComponent>(LAWN_X + PLOT_W * rnd.Int(0, COLS), SCREEN_HEIGHT, DRAW_LAYER_9 - 0.1f * layerId);
        sunEntity->assign<RenderComponent>(CreateRef<Sprite>(sunSprite));

        Collider* sunCollider = new CircleCollider(sunEntity, sun->GetWidth() / 2.4f);
        // Collider* sunCollider = new BoxCollider(sunEntity, 0, 0, (float)sun->GetWidth(), (float)sun->GetHeight());
        CollisionLayers collisionLayer = CollisionLayers::LAYER_1 << layerId;
        sunCollider->collisionLayer = collisionLayer | CollisionLayers::SUN;
        sunCollider->collidesWithLayers = collisionLayer | CollisionLayers::GROUND;
        sunCollider->debugColor = glm::vec4(0.45f, 0.0f, 0.5f, 1.0f);
        sunEntity->assign<ColliderComponent>(Ref<Collider>(sunCollider));

        sunEntity->assign<NativeScriptComponent>()->Bind<SunScript>();

        auto rigidBody = sunEntity->assign<RigidBodyComponent>();
        rigidBody->isKinematic = false;

        nextSpawnIn = spawnInterval;
    }
}
