#pragma once
#include "MouseClickEvent.h"
#include "KeyBoardEvent.h"
#pragma warning(push, 0)
#include "SFML/Graphics/RenderWindow.hpp"
#pragma warning(pop)
#include <functional>
#include <map>
#include "Player.h"

namespace TD
{
	class InputScanner
	{
	public:
		typedef std::function<void ()> KeyBoardAction;
		typedef std::function<void(sf::Mouse::Button button, int x, int y)> MouseClickAction;
		typedef std::function<void (int x, int y)> MouseMoveAction;

		InputScanner();

		bool init(sf::RenderWindow* renderWindow);

		bool update();

		void addKeyBoardAction(KeyBoardEvent event, KeyBoardAction action);

		void addMouseAction(MouseClickEvent event, MouseClickAction action);

		void addMouseMoveAction(MouseMoveAction action);

		void clearInputActions();

		bool isKeyBoardEvent(sf::Event& event);

		bool isMouseClickEvent(sf::Event& event);

		bool isMouseMoveEvent(sf::Event& event);

		bool isQuitEvent(sf::Event& event);
	
		sf::RenderWindow* getWindow() const { return this->m_inputSource; };

		~InputScanner() {};

	private:
		sf::RenderWindow* m_inputSource;
		std::map<KeyBoardEvent, KeyBoardAction> m_keyboardActions;
		std::map<MouseClickEvent, MouseClickAction> m_clickActions;
		std::vector<MouseMoveAction> m_moveActions;
	};
}