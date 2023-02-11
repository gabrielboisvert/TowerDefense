#pragma once
#include "Renderer.h"

namespace TD
{
	enum class TowerType
	{
		Regular,
		Explosive,
		Stun
	};

	static const char* TOWER_PATH[] = {
		"Assets/textures/PNG/Default size/towerDefense_tile249.png",
		"Assets/textures/PNG/Default size/towerDefense_tile206.png",
		"Assets/textures/PNG/Default size/towerDefense_tile291.png",
		
		"Assets/textures/PNG/Default size/towerDefense_tile250.png",
		"Assets/textures/PNG/Default size/towerDefense_tile205.png",
		"Assets/textures/PNG/Default size/towerDefense_tile292.png"
	};

	class ConfigTower
	{
	public:
		TowerType towerType;
		float fireRate;
		unsigned int damage;
		float range;
		//TD::BulletType bullet
		unsigned int maxLevel;

		ConfigTower();
		ConfigTower(TowerType towerType);

		void initRegular();
		void initExplosive();
		void initStun();

		void static initTowerImg(TD::Renderer& renderer);
	};
}