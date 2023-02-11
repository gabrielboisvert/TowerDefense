#include "pch.h"
#include "Window.h"


void TD::Window::init()
{
	this->m_window.create(sf::VideoMode(1200, 800), "SFML works!");
	this->m_window.setVerticalSyncEnabled(true);

	sf::Image img;
	img.loadFromFile("Assets/hud/PNG/cursor_pointerFlat.png");
	this->cursorDefault.loadFromPixels(img.getPixelsPtr(), img.getSize(), sf::Vector2u(0, 0));
	this->m_window.setMouseCursor(this->cursorDefault);

	img.loadFromFile("Assets/hud/PNG/cursor_hand.png");
	this->cursorAction.loadFromPixels(img.getPixelsPtr(), img.getSize(), sf::Vector2u(0, 0));
}

void TD::Window::quit()
{
	this->m_window.close();
}

bool TD::Window::isOpen()
{
	return this->m_window.isOpen();
}

bool TD::Window::pollEvent(sf::Event& event)
{
	return this->m_window.pollEvent(event);
}

void TD::Window::clear()
{
	this->m_window.clear();
}

void TD::Window::render(const sf::Drawable& sprite)
{
	this->m_window.draw(sprite);
}

void TD::Window::display()
{
	this->m_window.display();
}

void TD::Window::changeCursor(Cursor type)
{
	switch (type)
	{
	case Cursor::Default:	this->m_window.setMouseCursor(this->cursorDefault);
		break;
	case Cursor::Action:	this->m_window.setMouseCursor(this->cursorAction);
		break;
	}
}

sf::RenderWindow& TD::Window::getWindow()
{
	return this->m_window;
}