#include "pch.h"
#include "KeyBoardEvent.h"


TD::KeyBoardEvent::KeyBoardEvent(sf::Keyboard::Key key, sf::Event::EventType type)
{
	this->event = type;
	this->keyCode = key;
}

TD::KeyBoardEvent::KeyBoardEvent(sf::Event& event)
{
	this->keyCode = event.key.code;
	this->event = event.type;
}

bool TD::KeyBoardEvent::operator<(const KeyBoardEvent& keyboardEvent) const
{
	if (keyboardEvent.keyCode > this->keyCode)
		return true;
	return false;
}
