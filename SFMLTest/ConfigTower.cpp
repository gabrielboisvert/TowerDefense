#include "pch.h"
#include "ConfigTower.h"


TD::ConfigTower::ConfigTower()
{
	this->towerType = TowerType::Regular;
	this->initRegular();
}

TD::ConfigTower::ConfigTower(TowerType towerType)
{
	this->towerType = towerType;
	switch (towerType)
	{
	case TD::TowerType::Regular:	this->towerType = TowerType::Regular;
									this->initRegular();
		break;
	case TD::TowerType::Explosive:	this->towerType = TowerType::Explosive;
									this->initExplosive();
		break;
	case TD::TowerType::Stun:	this->towerType = TowerType::Stun;
								this->initStun();
		break;
	default:	this->towerType = TowerType::Regular;
				this->initRegular();
		break;
	}
}

void TD::ConfigTower::initRegular()
{
	this->towerType = TD::TowerType::Regular;
	this->fireRate = 0.5f;
	this->damage = 2;
	this->range = 150;
	this->maxLevel = 5;
}

void TD::ConfigTower::initExplosive()
{
	this->towerType = TD::TowerType::Explosive;
	this->fireRate = 1.5f;
	this->damage = 5;
	this->range = 120;
	this->maxLevel = 5;
}

void TD::ConfigTower::initStun()
{	
	this->towerType = TD::TowerType::Stun;
	this->fireRate = 0.7f;
	this->damage = 1;
	this->range = 135;
	this->maxLevel = 5;
}

void TD::ConfigTower::initTowerImg(TD::Renderer& renderer)
{
	for (unsigned int i = 0; i < 6; i++)
		renderer.LoadTextureFile(TD::TOWER_PATH[i]);
}