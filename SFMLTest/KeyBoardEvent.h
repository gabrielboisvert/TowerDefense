#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Event.hpp"
namespace TD 
{
	class KeyBoardEvent
	{
	public:
		KeyBoardEvent(sf::Keyboard::Key key, sf::Event::EventType type);
		KeyBoardEvent(sf::Event& event);

		sf::Keyboard::Key keyCode;
		sf::Event::EventType event;

		bool operator<(const KeyBoardEvent& keyboardEvent) const;
	};
}