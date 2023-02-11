#include "pch.h"
#include "IGameManager.h"


TD::IGameManager::IGameManager()
{
	this->deltaTime = 0;
	this->currentState = GameState::MAIN_MENU;
}

void TD::IGameManager::updateDeltaTime()
{
	this->deltaTime = this->clock.restart().asSeconds();
}

const float TD::IGameManager::getDeltaTime()
{
	return this->deltaTime;
}

void TD::IGameManager::setWindow(TD::Window& window)
{
	this->renderer.setWindow(window);
}