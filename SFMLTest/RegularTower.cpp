#include "pch.h"
#include "RegularTower.h"
#include <SFML/Graphics/CircleShape.hpp>


TD::RegularTower::RegularTower()
{
	this->level = 1;
	this->price = 50;
	this->sprite = nullptr;
	this->updateCost = 100;
	this->config.initRegular();
}

void TD::RegularTower::shootAt(std::shared_ptr<TD::Enemy*>& enemy, TD::Renderer& renderer)
{
	Bullet* bullet = new Bullet(this->config.towerType);
	bullet->enemy = enemy;
	
	sf::Vector2i bulletPos = sf::Vector2i(this->sprite->getPosition());
	
	bullet->sprite = &renderer.CreateSprite(*renderer.GetTexture(BULLET_TEXTURE[0]), bulletPos, 0);
	bullet->sprite->setOrigin(bullet->sprite->getLocalBounds().width / 2, bullet->sprite->getLocalBounds().height / 2);

	bullets.push_back(bullet);
}

void TD::RegularTower::levelUp(TD::Renderer& renderer, TD::Player& player)
{
	if ((int)player.money - (int)updateCost < 0 || this->level == this->config.maxLevel)
		return;
	
	if (this->sprite->getTexture() != renderer.GetTexture(TOWER_PATH[3]))
		this->sprite->setTexture(*renderer.GetTexture(TOWER_PATH[3]), true);
	
	this->level++;
	player.updateMoney(0 - this->updateCost);
	
	this->updateCost += price;

	this->config.range *= 1.05f;
	this->config.damage *= 2;
	this->config.fireRate *= 0.9f;
	this->updateCost += this->price;
	
	sf::CircleShape* shape = dynamic_cast<sf::CircleShape*>(this->sprite->shape);
	shape->setRadius(this->config.range);
	shape->setPosition(sf::Vector2f(this->sprite->getPosition().x - (this->sprite->shape->getGlobalBounds().width / 2), this->sprite->getPosition().y - (this->sprite->shape->getGlobalBounds().height / 2)));
}

void TD::RegularTower::update(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player)
{	
	this->updateBullets(delta, enemies, renderer, player);

	std::shared_ptr<TD::Enemy*>* enemy = this->focusTarget(delta, enemies);
	if (enemy == nullptr)
		return;

	shootAt(*enemy, renderer);
	this->fireCount = sf::Time();
}