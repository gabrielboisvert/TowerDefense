#pragma once
#include "Sprite.h"
#include "Renderer.h"
#include <functional>

static const char* BUTTON_TEXTURES[]
{
	"Assets/hud/PNG/dotRed.png",
	"Assets/hud/PNG/dotGreen.png",
	"Assets/hud/PNG/dotBlue.png",
	"Assets/hud/PNG/barHorizontal_yellow_mid.png",
	"Assets/hud/PNG/barHorizontal_blue_mid.png",
	"Assets/hud/PNG/barHorizontal_red_mid.png",
	"Assets/hud/PNG/pause-button.png",
	"Assets/hud/PNG/upgradeButton.png"
};

namespace TD {
	class HUBButton {
	public:
		TD::Sprite* sprite;
		TD::Sprite* hoveredSprite;
		TD::Sprite* clickedSprite;
		sf::Vector2i relativePosition;
		TD::Sprite* defaultSp = sprite;
		std::function<bool()> action;

		HUBButton();

		void setRelativePosition(sf::Vector2i& position, sf::Vector2i relativePos);
		
		~HUBButton();

		static void initTexture(TD::Renderer& renderer);
	};
}