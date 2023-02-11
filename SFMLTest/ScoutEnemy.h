#pragma once
#include "Enemy.h"

namespace TD
{
	class ScoutEnemy : public Enemy
	{
	private:
	public:
		ScoutEnemy();
		void update(const float deltaTime) override;
	};
}