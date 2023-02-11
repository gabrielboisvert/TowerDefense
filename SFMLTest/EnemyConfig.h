#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "Renderer.h"

namespace TD {
	enum class EnemyType {
		SCOUT = 0,
		TANK = 1,
		HEALER = 8
	};

	static const char* ENEMY_PATH[] = {
		"Assets/textures/PNG/Default size/towerDefense_tile245.png",
		"Assets/textures/PNG/Default size/towerDefense_tile246.png",
		"Assets/textures/PNG/Default size/towerDefense_tile247.png"
	};

	class EnemyConfig {
	public:
		EnemyType type;
		float speed;
		unsigned int damage;
		unsigned int gold;
		unsigned int life;
		unsigned int maxLife;

		EnemyConfig();
		EnemyConfig(EnemyType type);
		EnemyType getType() { return this->type; };
		
		void initScout();
		void initTank();
		void initHealer();

		static void initTexture(TD::Renderer& renderer);
	};
}