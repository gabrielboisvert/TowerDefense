#pragma once
#include "HUDButton.h"
#include <vector>

static const char* BACKGROUND_TEXTURES[]
{
	"Assets/hud/PNG/metalPanel.png",
	"Assets/logo.png",
	"Assets/hud/PNG/metalPanel_blueCorner.png",
	"Assets/hud/PNG/metalPanel_plate.png"
};

namespace TD {
	class HUBWindow {
	public:
		HUBWindow(unsigned int layer) { this->sprite = nullptr; this->layer = layer; };

		std::vector<HUBButton*> buttons;
		sf::Vector2i position;
		TD::Sprite* sprite;
		unsigned int layer;

		template <typename T> T* addButton()
		{
			T* button = new T;
			this->buttons.push_back(button);
			return button;
		}

		void clear();

		~HUBWindow();

		static void initBackgroundImg(TD::Renderer& renderer);
	};
}