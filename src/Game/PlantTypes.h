//#include "Game/Game.h"
#include "Core/AssetManager.h"
#include <vector>
enum class PlantTypes { 
	None, 
	Peashooter, 
	Sunflower, 
	Wallnut, 
	Tallnut 
};


struct Plant {
	int type;
	int cost;
	bool isShooter;
	bool isSpawningSuns;
	float health;
	Ref<TextureGPU> tex;
};

struct VecPlants
{
private:
	std::vector<Plant> plants = { { (int)PlantTypes::Peashooter, 100, true, false, 3, AssetManager::LoadCachedImageFile("assets/sprites/Peashooter.png")},
									{(int)PlantTypes::Sunflower, 50, false, true, 3, AssetManager::LoadCachedImageFile("assets/sprites/Sunflower.png")},
									{(int)PlantTypes::Wallnut, 50, false, false, 72, AssetManager::LoadCachedImageFile("assets/sprites/Wallnut1.png")},
									{(int)PlantTypes::Tallnut, 125, false, false, 144, AssetManager::LoadCachedImageFile("assets/sprites/Tallnut1.png")} };


public:
	Plant& GetPlantByType(int type)
	{
		for (auto& plant : plants)
		{
			if (plant.type == type)
			{
				return plant;
			}
		}
	}

	Ref<TextureGPU> GetPlantTexByType(int type)
	{
		for (auto& plant : plants)
		{
			if (plant.type == type)
			{
				return plant.tex;
			}
		}
	}

	bool isShooterByType(int type)
	{
		for (auto& plant : plants)
		{
			if (plant.type == type)
			{
				return plant.isShooter;
			}
		}
	}
};
