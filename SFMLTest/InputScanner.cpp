#include "pch.h"
#include "InputScanner.h"


TD::InputScanner::InputScanner()
{
    this->m_inputSource = nullptr;
}

bool TD::InputScanner::init(sf::RenderWindow* renderWindow)
{
	if (renderWindow == nullptr)
		return false;
	this->m_inputSource = renderWindow;
	return true;
}

bool TD::InputScanner::update()
{
    sf::Event event;
    while (this->m_inputSource->pollEvent(event))
    {
        if (this->isQuitEvent(event)) {
            this->m_inputSource->close();
            break;
        }
        else if (this->isKeyBoardEvent(event))
        {
            TD::KeyBoardEvent keyEvent = KeyBoardEvent(event);
            std::map<TD::KeyBoardEvent, TD::InputScanner::KeyBoardAction>::iterator it = this->m_keyboardActions.find(event);
            if ( it != this->m_keyboardActions.end())
                it->second();
        }
        else if (this->isMouseClickEvent(event))
        {
            TD::MouseClickEvent ClickEvent = MouseClickEvent(event);
            if (this->m_clickActions.find(ClickEvent) != this->m_clickActions.end())
                this->m_clickActions[ClickEvent](event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
        }
        else if (this->isMouseMoveEvent(event))
        {
            if (this->m_moveActions.size() > 0)
                this->m_moveActions[0](event.mouseMove.x, event.mouseMove.y);
        }
    }
	return true;
}


void TD::InputScanner::addKeyBoardAction(KeyBoardEvent event, KeyBoardAction action)
{
	this->m_keyboardActions.insert(std::pair<KeyBoardEvent, KeyBoardAction>(event, action));
}

void TD::InputScanner::addMouseAction(MouseClickEvent event, MouseClickAction action)
{
	this->m_clickActions.insert(std::pair<MouseClickEvent, MouseClickAction>(event, action));
}

void TD::InputScanner::addMouseMoveAction(MouseMoveAction action)
{
	this->m_moveActions.push_back(action);
}

void TD::InputScanner::clearInputActions()
{
	this->m_keyboardActions.clear();
	this->m_clickActions.clear();
	this->m_moveActions.clear();
}

bool TD::InputScanner::isKeyBoardEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::KeyPressed;
}

bool TD::InputScanner::isMouseClickEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::MouseButtonPressed || event.type == sf::Event::EventType::MouseButtonReleased;
}

bool TD::InputScanner::isMouseMoveEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::MouseMoved;
}

bool TD::InputScanner::isQuitEvent(sf::Event& event)
{
	return event.type == sf::Event::EventType::Closed;
}