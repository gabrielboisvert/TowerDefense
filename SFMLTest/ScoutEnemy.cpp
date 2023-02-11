#include "pch.h"
#include "ScoutEnemy.h"


TD::ScoutEnemy::ScoutEnemy()
{
	this->config.initScout();
}

void TD::ScoutEnemy::update(const float deltaTime)
{
	this->move(deltaTime);
}
