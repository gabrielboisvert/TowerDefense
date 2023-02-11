#pragma once
#include "Sprite.h"
#include <SFML/System/Vector2.hpp>
#include "Renderer.h"
#include "ConfigTower.h"
#include "EnemyArmy.h"
#include "Enemy.h"

namespace TD
{	
	class ITower;
	class Enemy;
	
	static const char* BULLET_TEXTURE[3]
	{
		"Assets/textures/PNG/Default size/towerDefense_tile295.png",
		"Assets/textures/PNG/Default size/towerDefense_tile251.png",
		"Assets/textures/PNG/Default size/towerDefense_tile297.png",
	};

	class Bullet
	{
	public:
		float Speed;
		sf::Vector2f direction;
		std::weak_ptr<Enemy*> enemy;
		TowerType type;
		Sprite* sprite;
		
		Bullet(TowerType bType);

		void update(const float& deltaTime, Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player);
		
		static void initTexture(Renderer& rend);

		~Bullet();

	private:
		void makeRegularDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*>);
	
		void makeAOEDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*>);
		
		void makeStunDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*>);

		
	};
}