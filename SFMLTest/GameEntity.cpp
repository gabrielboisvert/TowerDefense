#include "pch.h"
#include "GameEntity.h"


TD::GameEntity::GameEntity()
{
	this->sprite = nullptr;
}

TD::Sprite* TD::GameEntity::getSprite()
{
	return this->sprite;
}

void TD::GameEntity::setSprite(Sprite* sprite)
{
	this->sprite = sprite;
}