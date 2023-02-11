#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

namespace TD {
	enum class Cursor
	{
		Default,
		Action
	};

	class Window {
	private:
		sf::RenderWindow m_window;
		sf::Cursor cursorDefault;
		sf::Cursor cursorAction;
		
	public:
		void init();

		void quit();

		bool isOpen();

		bool pollEvent(sf::Event& event);

		void clear();

		void render(const sf::Drawable& sprite);

		void display();
		
		void changeCursor(Cursor type);

		sf::RenderWindow& getWindow();

		~Window() {};
	};
}