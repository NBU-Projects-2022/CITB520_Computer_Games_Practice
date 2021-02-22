#pragma once

#pragma warning(push, 0)
#include <imgui.h>
#pragma warning(pop)

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

#include "Core/Common.h"
#include "Core/Random.h"
#include "Rendering/Sprite.h"
#include "../Game.h"
#include "Physics/Colliders.h"


class NativeScript {
public:

    virtual void OnInit() {}
    virtual void OnDestroy() {}

    virtual void Update(float deltaTime) = 0;

    template<typename T>
    T& GetComponent()
    {
        return entity->get<T>().get();
    }

    template<typename T, typename... Args>
    T& AddComponent(Args &&... args) {
        return entity->assign<T>(std::forward<Args>(args)...).get();
    }

    ECS::Entity * CreateGameObject() {
        return entity->getWorld()->create();
    }

    ECS::Entity * GetEntity() {
        return entity;
    }

    bool shouldDestroy;
    float attackDamage;
    float hp;

private:
    ECS::Entity * entity;
    friend class NativeScriptSystem;
};


class PlantScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void OnDestroy() override;
    virtual void Update(float deltaTime) override;

private:
    bool placed;
    float x;
    float y;
    float spriteWidth;
    float spriteHeight;

    ECS::Entity *spawner;

    void PlacePlantLogic();
    void FixCollider(Collider * collider);
};

class PlantSpawnScript : public NativeScript
{
public:
    static bool shouldSpawn;
    static bool isHoldingPlant;
    static int plantType;

    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void OnSpawnPress();
};

class BulletScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    float bulletSpeed = 500.0f;
};

class BulletSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void SetCollisionLayer(CollisionLayers collisionLayer) {
        this->collisionLayer = collisionLayer;
    }

private:
    float spawnInterval = 2.f, nextSpawnIn = 1.5f;
    Ref<TextureGPU> bullet;
    Sprite bulletSprite;
    CollisionLayers collisionLayer;
};

class SunScript : public NativeScript
{
public:
    // virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
    void Collect();

private:
    int sunValue = 25;
    float sunFallSpeed = 100.0f;
    float sunFloatSpeed = 300.f;
    float sunFloatDistance = 100.f;
    float sunFloatCurrDistance = 0.f;
};

class SunSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void SetCollisionLayer(CollisionLayers collisionLayer) {
        this->collisionLayer = collisionLayer;
    }

private:
    float spawnInterval = 7.0f, nextSpawnIn = 1.0f;
    Ref<TextureGPU> sun;
    Sprite sunSprite;
    CollisionLayers collisionLayer;
};

class ZombieScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    float zombieSpeed = 15.0f;

    float attackSpeed = 3.0f;
    float nextAttackIn = 1.0f;
};

class ZombieSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void SpawnZombie();

    void SetCollisionLayer(int layerId) {
        this->layerId = layerId;
        collisionLayer = CollisionLayers::LAYER_1 << layerId;
    }

private:
    float waveInterval = 5.0f, nextWaveIn = 3.0f;
    Ref<TextureGPU> zombie;
    Sprite zombieSprite;
    CollisionLayers collisionLayer;
    int layerId;
};

class LawnMowerScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void SetCollisionLayer(int layerId) {
        this->layerId = layerId;
        collisionLayer = CollisionLayers::LAYER_1 << layerId;
    }

    void Activate() {
        active = true;
    }

private:
    Ref<TextureGPU> lawnMower;
    Sprite lawnMowerSprite;
    float speed = 400.f;
    bool active = false;
    CollisionLayers collisionLayer;
    int layerId;
};

class WaveControllerScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

    void AddZombies(float initialZombieTime = 3.0f);
    void AddWave();

private:
    Random rnd;
    const int ZOMBIES_COUNT = 5;
    const int WAVE_ZOMBIES_COUNT = 10;
    const float WAVE_DURATION_SPAN = 5.0f;
};

class SunCollector : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
};

class ShovelControllerScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    bool usedShovel = false;
};
