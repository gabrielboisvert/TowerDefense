#include "pch.h"
#include "HealerEnemy.h"
#include <SFML/Graphics/CircleShape.hpp>


TD::HealerEnemy::HealerEnemy()
{
	this->healRange = 75;
	this->healAmount = 3;
	this->healCooldown = sf::seconds(2.5);
	this->healingShapeCoolDown = sf::milliseconds(500);
	this->config.initHealer();
	this->isHealing = false;
}

void TD::HealerEnemy::updateHeal(const float deltaTime)
{
	this->heal += sf::seconds(deltaTime);
	this->healShape += sf::seconds(deltaTime);

	if (this->heal >= this->healCooldown)
	{
		this->heal = sf::seconds(0);
		this->isHealing = true;
		this->healShape = sf::seconds(0);

		this->healAround();
	}

	if (this->healShape >= this->healingShapeCoolDown)
		this->isHealing = false;

	if (this->isHealing)
		this->sprite->shape->setFillColor(sf::Color(0, 255, 0, 50));
	else
		this->sprite->shape->setFillColor(sf::Color(255, 255, 255, 50));
	
	sf::Vector2f pos = this->sprite->getPosition();
	pos.x -= this->healRange;
	pos.y -= this->healRange;
	this->sprite->shape->setPosition(pos);
}

void TD::HealerEnemy::healAround()
{
	bool hasHealSomeone = false;
	for (unsigned int i = 0; i < this->army->getArmy().size(); i++)
	{
		TD::Enemy* en = *this->army->getArmy()[i].get();
		if (this == en || !en->isSpawn())
			continue;

		if (this->sprite->getGlobalBounds().intersects(en->getSprite()->getGlobalBounds()))
		{
			if (en->getConfig().life == en->getConfig().maxLife)
				continue;

			if (en->getConfig().life + this->healAmount > en->getConfig().maxLife)
				en->getConfig().life = en->getConfig().maxLife;
			else
				en->getConfig().life += this->healAmount;
			
			en->getIsHeal() = true;
			hasHealSomeone = true;
		}
	}

	if (!hasHealSomeone)
	{
		if (this->config.life == this->config.maxLife)
			return;

		if (this->config.life + this->healAmount > this->config.maxLife)
			this->config.life = this->config.maxLife;
		else
			this->config.life += this->healAmount;
		this->isHeal = true;
	}
}

void TD::HealerEnemy::update(const float deltaTime)
{	
	this->updateHeal(deltaTime);
	this->move(deltaTime);
}

void TD::HealerEnemy::setRangeHeal()
{
	this->sprite->shape = new sf::CircleShape(this->healRange);
	this->sprite->drawShape = true;
	this->sprite->shape->setFillColor(sf::Color(255, 255, 255, 50));

	sf::Vector2f pos = this->sprite->getPosition();
	pos.x -= this->healRange;
	pos.y -= this->healRange;
	this->sprite->shape->setPosition(pos);
}
