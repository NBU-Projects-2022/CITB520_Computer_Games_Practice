#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"
#include "Rendering/Sprite.h"
#include "../Game.h"
#include <imgui.h>

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

    bool shouldDestroy;

private:
    ECS::Entity * entity;
    friend class NativeScriptSystem;
};

enum class PlantTypes { None, Peashooter, Sunflower, Wallnut, Tallnut };

class PlantScript : public NativeScript 
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
    
private:
    float x;
    float y;
    float spriteWidth;
    float spriteHeight;

    bool placed;
    ImGuiIO io;
};

class PlantSpawnScript : public NativeScript 
{
public:
    static bool shouldSpawn;
    static int plantType;
    
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void OnSpawnPress();

private:
    Ref<TextureGPU> peashooter;
    Ref<TextureGPU> sunflower;
    Ref<TextureGPU> wallnut;
    Ref<TextureGPU> tallnut;

    Sprite peashooterSprite;
    Sprite sunflowerSprite;
    Sprite wallnutSprite;
    Sprite tallnutSprite;
};

class BulletScript : public NativeScript
{
public:
   // virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    float bulletSpeed = 500.0f;
};

class BulletSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    float spawnInterval = .5f, nextSpawnIn = 1.0f;
    Ref<TextureGPU> bullet;
    Sprite bulletSprite;
};

class ZombieScript : public NativeScript 
{
public:
    virtual void Update(float deltaTime) override;

private:
    float zombieSpeed = 15.0f;
};

class ZombieSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    float waveInterval = 5.0f, nextWaveIn = 3.0f;
    Ref<TextureGPU> zombie;
    Sprite zombieSprite;
};

class LawnMowerScript : public NativeScript 
{
public:
    virtual void Update(float deltaTime) override;
};
