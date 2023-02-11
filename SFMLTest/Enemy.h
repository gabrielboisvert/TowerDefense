#pragma once
#include "GameEntity.h"
#include "EnemyConfig.h"
#include "EnemyArmy.h"
#include "PathFinding.h"

namespace TD {
	class EnemyArmy;

	class Enemy : public GameEntity {
	protected:
		EnemyConfig config;
		TD::EnemyArmy* army;
		unsigned int cellIdx;

		sf::Time healCoolDown;
		sf::Time healCount;
		bool isHeal;

		float slowCoef;
		sf::Time slowCoolDown;
		sf::Time slowCount;

		bool isHit;
		sf::Time hitCoolDown;
		sf::Time hitTimer;

		sf::Time spawnTimer;
		sf::Time spawnCounter;
		bool hasSpawn;
	public:
		Enemy();
		Enemy(EnemyType type, TD::EnemyArmy& army);
		void setArmy(EnemyArmy& army);
		EnemyType getType() { return this->config.getType(); };
		void setCellIdx(unsigned int cellIdx) { this->cellIdx = cellIdx; };
		virtual void update(const float deltaTime) = 0;
		bool operator==(Enemy& en);
		void move(const float deltaTime);
		
		void setSlow(const float slowCoef, sf::Time slowCoolDown);
		void spawn() { this->hasSpawn = true; };
		bool isSpawn() { return this->hasSpawn; };
		void setSpawnTimer(sf::Time timer) { this->spawnTimer = timer; };

		void addTime(const float& delta);
		sf::Time& getSpawnTimer() { return this->spawnTimer; };
		sf::Time& getSpawnCounter() { return this->spawnCounter; };
		
		void hit() { this->isHit = true; };

		void updateHeal(const float deltaTime);
		void updateColor();
		EnemyConfig& getConfig();
		bool& getIsHeal();
		
		~Enemy();
	};
}