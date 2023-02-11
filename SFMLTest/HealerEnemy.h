#pragma once
#include <SFML\System\Time.hpp>
#include "Enemy.h"

namespace TD
{
	class HealerEnemy : public Enemy
	{
	private:
		float healRange;
		unsigned int healAmount;
		sf::Time healCooldown;
		sf::Time healingShapeCoolDown;
		
		sf::Time heal;
		sf::Time healShape;
		bool isHealing;

		void updateHeal(const float deltaTime);
	public:
		HealerEnemy();
		void healAround();
		void update(const float deltaTime) override;
		void setRangeHeal();
	};
}