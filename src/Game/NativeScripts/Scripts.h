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

    void Damage(float dmg) { hp -= dmg; }
    int GetPrice() { return price; }

private:
    bool placed;
    int price;
    float x;
    float y;
    float spriteWidth;
    float spriteHeight;
    float hp;

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
    float attackDamage = 1.f;
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
    float flyTime = 0.5f;
    int sunValue = 25;
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
    Random rnd;
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
    virtual void OnDestroy() override;

    void Damage(float dmg) { hp -= dmg; }

private:
    float zombieSpeed = PLOT_W / 4.7f;

    float attackSpeed = 1.0f;
    float nextAttackIn = 1.0f;
    float attackDamage = 10.f ;
    float hp = 2.f;
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
    const float WAVE_DURATION_SPAN = 15.0f;
    const float WAVE_INTERVAL = 45.0f;
};

class SunCollector : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
};

class GameLostScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
};

class GlobalSunSpawnScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    Random rnd;
    float spawnInterval = 15.0f, nextSpawnIn = 5.0f;
    Ref<TextureGPU> sun;
    Sprite sunSprite;
};

class ShovelControllerScript : public NativeScript
{
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;

private:
    bool usedShovel = false;
};
