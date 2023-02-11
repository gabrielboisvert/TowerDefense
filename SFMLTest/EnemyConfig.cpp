#include "pch.h"
#include "EnemyConfig.h"


TD::EnemyConfig::EnemyConfig()
{
	this->initScout();
}

TD::EnemyConfig::EnemyConfig(EnemyType type)
{
	this->type = type;
	switch (type)
	{
	case TD::EnemyType::SCOUT:	this->initScout();
		break;
	case TD::EnemyType::TANK:	this->initTank();
		break;
	case TD::EnemyType::HEALER: this->initHealer();
		break;
	default:	this->initScout();
		break;
	}
}

void TD::EnemyConfig::initScout()
{
	this->type = TD::EnemyType::SCOUT;
	this->speed = 40;
	this->damage = 5;
	this->gold = 25;
	this->life = 10;
	this->maxLife = 15;
}

void TD::EnemyConfig::initTank()
{
	this->type = TD::EnemyType::TANK;
	this->speed = 30;
	this->damage = 15;
	this->gold = 100;
	this->life = 30;
	this->maxLife = 45;
}

void TD::EnemyConfig::initHealer()
{
	this->type = TD::EnemyType::HEALER;
	this->speed = 50;
	this->damage = 2;
	this->gold = 35;
	this->life = 10;
	this->maxLife = 15;
}

void TD::EnemyConfig::initTexture(TD::Renderer& renderer)
{
	for (unsigned int i = 0; i < 3; i++)
		renderer.LoadTextureFile(ENEMY_PATH[i]);
}