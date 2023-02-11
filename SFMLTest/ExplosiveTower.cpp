#include "pch.h"
#include "ExplosiveTower.h"
#include <SFML/Graphics/CircleShape.hpp>


TD::ExplosiveTower::ExplosiveTower()
{
	this->level = 1;
	this->price = 95;
	this->sprite = nullptr;
	this->updateCost = 100;
	this->config.initExplosive();
	this->bulletRadius = 75;
}

void TD::ExplosiveTower::shootAt(std::shared_ptr<TD::Enemy*>& enemy, TD::Renderer& renderer)
{
	Bullet* bullet = new Bullet(this->config.towerType);
	bullet->enemy = enemy;

	sf::Vector2i bulletPos = sf::Vector2i(this->sprite->getPosition());

	bullet->sprite = &renderer.CreateSprite(*renderer.GetTexture(BULLET_TEXTURE[1]), bulletPos, 0);
	bullet->sprite->setOrigin(bullet->sprite->getLocalBounds().width / 2, bullet->sprite->getLocalBounds().height / 2);

	bullet->sprite->shape = new sf::CircleShape(float(this->bulletRadius));
	bullet->sprite->shape->setFillColor(sf::Color(255, 255, 255, 100));
	bullet->sprite->shape->setPosition(sf::Vector2f(bullet->sprite->getPosition().x - this->bulletRadius, bullet->sprite->getPosition().y - this->bulletRadius));
	bullet->sprite->drawShape = true;

	bullets.push_back(bullet);
}


void TD::ExplosiveTower::levelUp(TD::Renderer& renderer, TD::Player& player)
{
	if ((int)player.money - (int)updateCost < 0 || this->level == this->config.maxLevel)
		return;

	if (this->sprite->getTexture() != renderer.GetTexture(TOWER_PATH[4]))
		this->sprite->setTexture(*renderer.GetTexture(TOWER_PATH[4]), true);

	this->sprite->setOrigin(this->sprite->getLocalBounds().width / 2, this->sprite->getLocalBounds().height / 2);

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

void TD::ExplosiveTower::update(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player)
{
	this->updateBullets(delta, enemies, renderer, player);

	std::shared_ptr<TD::Enemy*>* enemy = this->focusTarget(delta, enemies);
	if (enemy == nullptr)
		return;

	shootAt(*enemy, renderer);
	this->fireCount = sf::Time();
}