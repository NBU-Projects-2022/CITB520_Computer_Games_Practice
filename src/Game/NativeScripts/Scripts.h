#pragma once

#include <ECS.h>
ECS_TYPE_IMPLEMENTATION;

// #include "ComponentSystem/Components.h"
#include "Core/Common.h"
#include "Rendering/Sprite.h"
#include "../Game.h"


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

enum class PlantTypes { None, Peashooter, Sunflower, Wallnut, Tallnut };
//int plantType = (int)PlantTypes::None;
//bool shouldSpawn = false;
class PlantScript : public NativeScript {
public:
    virtual void OnInit() override;
    virtual void Update(float deltaTime) override;
    PlantScript() = default;
    PlantScript(PlantTypes, float, float);

    
private:
    float x, y;
   
};


    //
    //
    //
    //
   // 
   // 
   // 

class PlantSpawnScript : public NativeScript {
public:
    //static
  //  static
    
    virtual void OnInit() override;

    virtual void Update(float deltaTime) override;
    void OnSpawnPress();
    static int plantType;
    static bool shouldSpawn;
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
