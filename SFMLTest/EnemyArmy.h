#pragma once
#include <vector>
#include "Enemy.h"
#include "GameMap.h"
#include "Player.h"
#include <SFML/Graphics/Text.hpp>


namespace TD {
	class Enemy;
	class GameMap;

	class EnemyArmy {
	private:
		std::vector<std::shared_ptr<TD::Enemy*>> enemies;
		TD::GameMap* map;
		TD::Player* player;
		TD::Renderer* renderer;
		unsigned int wave;
		sf::Text* waveText;

	public:
		template <typename T> T* spawnEnemy()
		{
			T* enemy = new T();
			this->enemies.push_back(std::make_shared<TD::Enemy*>(enemy));
			enemy->setArmy(*this);
			return enemy;
		}

		EnemyArmy();

		void removeEnemy(Enemy& en);

		void clearAllEnemy();

		void reset();

		std::vector<std::shared_ptr<TD::Enemy*>>& getArmy();

		TD::GameMap* getMap() { return this->map; };

		TD::Player* getPlayer() { return this->player; };

		TD::Renderer* getRenderer() { return this->renderer; };

		unsigned int getWave() { return this->wave; };

		void updateWave();

		void setWaveText(sf::Text* txt);

		void initSprites(TD::GameMap& map, TD::Renderer& renderer, TD::Player& player);

		void initSprite(TD::GameMap& map, TD::Renderer& renderer, TD::Player& player, unsigned int idx);

		void fillSprite(TD::Renderer& renderer);

		~EnemyArmy();
	};
}