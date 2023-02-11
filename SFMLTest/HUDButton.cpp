#include "pch.h"
#include "HUDButton.h"


TD::HUBButton::HUBButton()
{
	this->sprite = nullptr;
	this->clickedSprite = new Sprite;
	this->hoveredSprite = new Sprite;
	this->defaultSp = new Sprite;
}

void TD::HUBButton::setRelativePosition(sf::Vector2i& position, sf::Vector2i relativePos)
{
	this->relativePosition = sf::Vector2i(relativePos.x + position.x, relativePos.y + position.y);
}

TD::HUBButton::~HUBButton()
{
	delete this->hoveredSprite;
	delete this->clickedSprite;
	delete this->defaultSp;
}

void TD::HUBButton::initTexture(TD::Renderer& renderer)
{
	for (unsigned int i = 0; i < 8; i++)
		renderer.LoadTextureFile(BUTTON_TEXTURES[i]);
}