#include "pch.h"
#include "ITower.h"
#include "SFML/Graphics/CircleShape.hpp"


TD::ITower::ITower()
{
    this->price = 0;
    this->level = 1;
    this->updateCost = 0;
    this->hasBeenPlace = false;
}

void TD::ITower::initRadius()
{
	sf::CircleShape* radius = new sf::CircleShape(this->config.range);
	radius->setFillColor(sf::Color(255, 255, 255, 100));
	this->sprite->shape = radius;
}

void TD::ITower::activate()
{
    this->hasBeenPlace = true;
}

std::shared_ptr<TD::Enemy*>* TD::ITower::focusTarget(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies)
{
    if (!this->hasBeenPlace)
        return nullptr;

    this->sprite->rotate(2);

    std::shared_ptr<TD::Enemy*>* enemy = this->checkRange(enemies);
    if (enemy == nullptr)
        return nullptr;

    sf::Vector2f pos = (*enemy->get())->getSprite()->getPosition();
    sf::Vector2f thisPos = this->sprite->getPosition();
    float angle = atan2(pos.y - thisPos.y, pos.x - thisPos.x);

    this->sprite->setRotation(float((double(angle) * 180 / M_PI) + 90));

    this->fireCount += sf::seconds(delta);
    if (this->fireCount < sf::seconds(this->config.fireRate))
        return nullptr;
    return enemy;
}

void TD::ITower::updateBullets(const float& delta, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Renderer& renderer, TD::Player& player)
{
    for (unsigned int i = 0; i < this->bullets.size(); i++)
        this->bullets[i]->update(delta, renderer, this, enemies, player);
}

TD::ITower::~ITower()
{
    for (unsigned int i = 0; i < this->bullets.size(); i++)
    {
        delete this->bullets[i];
    }
}

std::shared_ptr<TD::Enemy*>* TD::ITower::checkRange(std::vector<std::shared_ptr<TD::Enemy*>>& enemies)
{
    std::shared_ptr<TD::Enemy*>* closestEnemy = nullptr;
    float closestDistant = float(INT_MAX);

    sf::Vector2f towerPos = this->sprite->getPosition();
    for (unsigned int i = 0; i < enemies.size(); i++)
    {
        if (!(*enemies[i].get())->isSpawn())
            continue;

        sf::Vector2f enPos = (*enemies[i].get())->getSprite()->getPosition();
        float distance = sqrt((enPos.x - towerPos.x) * (enPos.x - towerPos.x) + (enPos.y - towerPos.y) * (enPos.y - towerPos.y));
        if (distance > this->config.range || closestDistant < distance)
            continue;
        
        closestEnemy = &enemies[i];
        closestDistant = distance;
    }
    return closestEnemy;
}
