#pragma once
#include "GameEntity.h"
#include "ConfigTower.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#define _USE_MATH_DEFINES
#include <Math.h>

namespace TD
{
	class Enemy;
	class Bullet;

	class ITower : public GameEntity
	{
	public:
		unsigned int price;
		std::uint8_t level;
		unsigned int updateCost;
		ConfigTower config;
		sf::Time fireCount;
		std::vector<TD::Bullet*> bullets;
		bool hasBeenPlace;

		ITower();
		
		void initRadius();
		void activate();
		
		std::shared_ptr<TD::Enemy*>* focusTarget(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies);
		
		virtual void shootAt(std::shared_ptr<TD::Enemy*>& enemy, TD::Renderer& renderer) = 0;
		virtual void levelUp(TD::Renderer& renderer, TD::Player& player) = 0;
		virtual void update(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player) = 0;
		
		void updateBullets(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player);

		~ITower();

	protected:
		std::shared_ptr<TD::Enemy*>* checkRange(std::vector<std::shared_ptr<TD::Enemy*>>& enemies);


	};
}