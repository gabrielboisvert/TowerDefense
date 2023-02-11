#pragma once
#pragma warning(push, 0)
#include "SFML/Window/Mouse.hpp"
#include "SFML/Window/Event.hpp"
#pragma warning(pop)
namespace TD {
	class MouseClickEvent 
	{
	public:
		MouseClickEvent(sf::Mouse::Button button, sf::Event::EventType type);
		
		MouseClickEvent(sf::Event& event);

		sf::Mouse::Button button;
		sf::Event::EventType event;

		bool operator<(const MouseClickEvent& mouseClickEvent) const;
	};
}