#include "pch.h"
#include "Sprite.h"


TD::Sprite::Sprite()
{
	this->layer = 0;
	this->shape = nullptr;
	this->drawShape = false;
}

TD::Sprite::Sprite(Layer layer)
{
	this->layer = layer;
	this->shape = nullptr;
	this->drawShape = false;
}

TD::Sprite::~Sprite()
{
	delete this->shape;
}
