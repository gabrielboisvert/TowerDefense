#include "pch.h"
#include "Enemy.h"


TD::Enemy::Enemy()
{
	this->healCoolDown = sf::milliseconds(500);
	this->isHeal = false;

	this->sprite = nullptr;
	this->army = nullptr;
	this->cellIdx = 0;

	this->slowCoef = 0;
	this->hasSpawn = false;

	this->hitCoolDown = sf::milliseconds(150);
	this->isHit = false;
}

TD::Enemy::Enemy(EnemyType type, TD::EnemyArmy& army)
{	
	this->healCoolDown = sf::milliseconds(500);
	this->config = EnemyConfig(type);
	this->sprite = nullptr;
	this->cellIdx = 0;
	this->army = &army;
	this->isHeal = false;
	this->slowCoef = 0;
	this->hasSpawn = false;

	this->hitCoolDown = sf::milliseconds(300);
	this->isHit = false;
}

void TD::Enemy::setArmy(TD::EnemyArmy& army)
{
	this->army = &army;
}

bool TD::Enemy::operator==(Enemy& en)
{
	return this == &en;
}

void TD::Enemy::move(const float deltaTime)
{
	if (this->sprite == nullptr || !this->hasSpawn)
		return;

	if (this->slowCoef != 0)
	{
		this->slowCount += sf::seconds(deltaTime);
		if (this->slowCount >= this->slowCoolDown)
		{
			this->slowCoef = 0;
			this->slowCount = sf::Time();
			this->slowCoolDown = sf::Time();
		}
	}

	if (this->isHit)
	{
		this->hitTimer += sf::seconds(deltaTime);
		if (this->hitTimer >= this->hitCoolDown)
		{
			this->isHit = false;
			this->hitTimer = sf::Time();
		}
	}

	TD::GameMap& map = *this->army->getMap();
	ai::Node::EdgeList& path = map.getPath();
	if (path.size() <= this->cellIdx + double(1))
	{
		TD::Player* player = this->army->getPlayer();
		
		player->updateLife(0 - this->config.damage);

		int baseHealt = player->baseHealth;
		int baseDamage = this->config.damage;
		if (baseHealt - baseDamage < 0)
			player->baseHealth = 0;
		else
			player->baseHealth -= this->config.damage;


		this->army->removeEnemy(*this);
		this->army->getRenderer()->RemoveSprite(*this->sprite);
		delete this;
		return;
	}

	sf::Vector2i nextPos = map.getMapPathIdx(path[this->cellIdx + __int64(1)].getTo()->getIndex());
	sf::Vector2f cellPos = map.getCells()[nextPos.x][nextPos.y]->getSprite().getPosition();
	cellPos.x += map.getCellSize().x / 2;

	sf::Vector2i currentpos = sf::Vector2i(this->sprite->getPosition());
	currentpos.y -= (int)(map.getCellSize().y / 2);

	float speed = (this->config.speed - this->slowCoef) * deltaTime;

	if (currentpos.x < cellPos.x)
	{
		this->sprite->move(speed, 0);
		this->sprite->setRotation(0);
	}
	else if (currentpos.x > cellPos.x)
	{
		this->sprite->move(-speed, 0);
		this->sprite->setRotation(-180);
	}
	else if (currentpos.y < cellPos.y)
	{
		this->sprite->move(0, speed);
		this->sprite->setRotation(90);
	}
	else if (currentpos.y > cellPos.y)
	{
		this->sprite->move(0, -speed);
		this->sprite->setRotation(-90);
	}
	else
		this->cellIdx++;

	this->updateHeal(deltaTime);
	this->updateColor();
}

void TD::Enemy::setSlow(const float slowCoef, sf::Time slowCoolDown)
{
	this->slowCoef = slowCoef;
	this->slowCoolDown = slowCoolDown;
	this->slowCount = sf::Time();
}

void TD::Enemy::addTime(const float& delta)
{
	this->spawnCounter += sf::seconds(delta);
}

void TD::Enemy::updateHeal(const float deltaTime)
{
	if (this->healCount > this->healCoolDown)
	{
		this->healCount = sf::seconds(0);
		this->isHeal = false;
	}

	if (this->isHeal)
		this->healCount += sf::seconds(deltaTime);
}

void TD::Enemy::updateColor()
{
	if (this->isHit)
		this->sprite->setColor(sf::Color(255, 0, 0));
	else if (this->slowCoef != 0)
		this->sprite->setColor(sf::Color(0, 0, 255));
	else if (this->isHeal)
			this->sprite->setColor(sf::Color(0, 255, 0));
	else
		this->sprite->setColor(sf::Color(255, 255, 255));
}

TD::EnemyConfig& TD::Enemy::getConfig()
{
	return this->config;
}

bool& TD::Enemy::getIsHeal()
{
	return this->isHeal;
}

TD::Enemy::~Enemy()
{}
