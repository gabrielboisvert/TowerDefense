#include "pch.h"
#include "Player.h"


TD::Player::Player()
{
	this->money = 0;
	this->baseHealth = 0;
	this->score = 0;
	this->lifeText = nullptr;
	this->cashText = nullptr;
	this->scoreText = nullptr;
}

void TD::Player::updateLife(int amount)
{
	if ((int)this->baseHealth + amount < 0)
		this->baseHealth = 0;
	else
		this->baseHealth += amount;

	if (this->lifeText != nullptr)
		this->lifeText->setString(std::to_string(this->baseHealth) + " HP");
}

void TD::Player::updateMoney(int amount)
{
	this->money += amount;

	if (this->cashText != nullptr)
		this->cashText->setString(std::to_string(this->money) + " $");
}

void TD::Player::updateScore(unsigned int amount)
{
	this->score += amount;

	if (this->scoreText != nullptr)
		this->scoreText->setString(std::to_string(this->score) + " Points");
}

void TD::Player::setLifeText(sf::Text* txt)
{
	if (txt == nullptr)
		return;
	this->lifeText = txt;
}

void TD::Player::setCashText(sf::Text* txt)
{
	if (txt == nullptr)
		return;
	this->cashText = txt;
}

void TD::Player::setScoreText(sf::Text* txt)
{
	if (txt == nullptr)
		return;
	this->scoreText = txt;
}

template<typename T> T* TD::Player::buyTower()
{
	return nullptr;
}

void TD::Player::reset()
{
	this->score = 0;
}

void TD::Player::clear()
{
	this->hub.clearWindow();
}