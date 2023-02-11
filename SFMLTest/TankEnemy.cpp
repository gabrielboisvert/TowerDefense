#include "pch.h"
#include "TankEnemy.h"


TD::TankEnemy::TankEnemy()
{
	this->config.initTank();
}

void TD::TankEnemy::update(const float deltaTime)
{
	this->move(deltaTime);
}
