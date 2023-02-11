#pragma once
#include "ITower.h"

namespace TD
{
	class ExplosiveTower : public ITower
	{
	private:
		unsigned int bulletRadius;
	public:
		ExplosiveTower();
		void shootAt(std::shared_ptr<TD::Enemy*>& enemy, TD::Renderer& renderer) override;
		void levelUp(TD::Renderer& renderer, TD::Player& player) override;
		void update(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player) override;
		unsigned int getBulletRadius() { return this->bulletRadius; };
	};
}