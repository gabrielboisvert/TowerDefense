#pragma once
#include "Enemy.h"

namespace TD
{
	class TankEnemy : public Enemy
	{
	private:
	public:
		TankEnemy();
		void update(const float deltaTime) override;
	};
}