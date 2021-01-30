#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

// #include "ComponentSystem/Components.h"
#include "Core/Common.h"
#include "Rendering/Sprite.h"

class NativeScript {
public:

    virtual void OnInit() {}
    virtual void Update(float deltaTime) = 0;

    template<typename T>
    T& GetComponent()
    {
        return entity->get<T>().get();
    }

    ECS::Entity * CreateGameObject() {
        return entity->getWorld()->create();
    }

private:
    ECS::Entity * entity;
    friend class NativeScriptSystem;
};

class PlantScript : public NativeScript {
public:
    virtual void Update(float deltaTime) override;
};

class ZombieScript : public NativeScript {
public:
    virtual void Update(float deltaTime) override;
private:
    float zombieSpeed = 15.0f;
};

class ZombieSpawnScript : public NativeScript {
public:
    virtual void OnInit() override;

    virtual void Update(float deltaTime) override;

private:
    float waveInterval = 5.0f, nextWaveIn = 3.0f;
    Ref<TextureGPU> zombie;
    Sprite zombieSprite;
};

class LawnMowerScript : public NativeScript {
public:
    virtual void Update(float deltaTime) override;
};
