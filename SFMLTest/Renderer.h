#pragma once
#include <map>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Sprite.h"
#include "Window.h"
#include <list>


namespace TD {
	class Renderer {
	private:
		std::map<std::string, sf::Texture*> textures;
		std::vector<std::list<TD::Sprite*>> sprites;
		std::vector<sf::Text*> texts;
		Window* window;
		sf::Font* font;
	public:
		Renderer();
		TD::Sprite& CreateSprite(const sf::Texture& my_Texture, sf::Vector2i position, Sprite::Layer zLayer = 0);
		void RemoveSprite(TD::Sprite& my_Sprite);
		void removeText(sf::Text& text);
		void addSprite(TD::Sprite& my_Sprite, Sprite::Layer zLayer = 0);
		void clear();
		const sf::Texture* LoadTextureFile(const std::string& name);
		void loadFont(std::string path);
		sf::Text* createText(unsigned int size, std::string str, sf::Color color, sf::Vector2f position);
		const sf::Texture* GetTexture(const std::string& name);
		void DrawSprites();
		void setWindow(Window& window);
		Window* getWindow();
		void freeTexture();
		~Renderer();
	};

	
}