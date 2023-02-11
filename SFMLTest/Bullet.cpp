#include "pch.h"
#include "Bullet.h"
#include "ITower.h"
#include "StunTower.h"
#include "ExplosiveTower.h"

TD::Bullet::Bullet(TowerType bType)
{
	this->type = bType;
	this->Speed = 0;

	if (this->type == TD::TowerType::Regular)
		this->Speed = 500;
	else if (this->type == TD::TowerType::Explosive)
		this->Speed = 500;
	else if (this->type == TD::TowerType::Stun)
		this->Speed = 500;

	this->sprite = nullptr;
}

void TD::Bullet::update(const float& deltaTime, Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player)
{
	std::shared_ptr<TD::Enemy*> tmpEnemy = this->enemy.lock();
	//Enemy already killed, so remove bullet
	if (!tmpEnemy)
	{
		renderer.RemoveSprite(*this->sprite);
		tower->bullets.erase(std::find(tower->bullets.begin(), tower->bullets.end(), this));
		delete this->sprite;
		delete this;
		return;
	}

	sf::Vector2f centerEnemy = (*tmpEnemy.get())->getSprite()->getPosition();
	float angle = atan2(centerEnemy.y - this->sprite->getPosition().y, centerEnemy.x - this->sprite->getPosition().x);
	this->sprite->setRotation(float(((double)angle * 180 / M_PI) + 90));
	
	this->direction = sf::Vector2f(cos(angle), sin(angle));
	this->sprite->move((direction.x * this->Speed) * deltaTime, (direction.y * this->Speed) * deltaTime);
	if (this->sprite->shape != nullptr)
		this->sprite->shape->move((direction.x * this->Speed) * deltaTime, (direction.y * this->Speed) * deltaTime);

	sf::Rect<float> bulletHitBox = this->sprite->getGlobalBounds();
	bulletHitBox.left += bulletHitBox.width / 3;
	bulletHitBox.width /= 3;

	bulletHitBox.top += bulletHitBox.height / 3;
	bulletHitBox.height /= 3;

	if (bulletHitBox.contains(centerEnemy))
	{
		switch (this->type)
		{
		case TD::TowerType::Regular:	this->makeRegularDamage(renderer, tower, enemies, player, tmpEnemy);
			break;
		case TD::TowerType::Explosive:	this->makeAOEDamage(renderer, tower, enemies, player, tmpEnemy);
			break;
		case TD::TowerType::Stun:	this->makeStunDamage(renderer, tower, enemies, player, tmpEnemy);
			break;
		}

		// Clear bullet
		renderer.RemoveSprite(*this->sprite);
		tower->bullets.erase(std::find(tower->bullets.begin(), tower->bullets.end(), this));
		delete this->sprite;
		delete this;
	}
}

void TD::Bullet::initTexture(Renderer& renderer)
{
	for (unsigned int i = 0; i < 3; i++)
		renderer.LoadTextureFile(BULLET_TEXTURE[i]);
}

TD::Bullet::~Bullet() {}

void TD::Bullet::makeRegularDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*> enemy)
{
	if ((int)(*enemy.get())->getConfig().life - (int)tower->config.damage < 0)
		(*enemy.get())->getConfig().life = 0;
	else
		(*enemy.get())->getConfig().life -= tower->config.damage;
	(*enemy.get())->hit();

	if ((*enemy.get())->getConfig().life == 0)
	{
		player.updateScore((*enemy.get())->getConfig().gold);
		player.updateMoney((*enemy.get())->getConfig().gold);

		renderer.RemoveSprite(*(*enemy.get())->getSprite());
		std::vector<std::shared_ptr<TD::Enemy*>>::iterator enemyIt = std::find(enemies.begin(), enemies.end(), enemy);
		if (enemyIt != enemies.end())
			enemies.erase(enemyIt);
		delete (*enemy.get())->getSprite();
		delete *enemy.get();
	}
}

void TD::Bullet::makeAOEDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*> enemy)
{
	sf::Vector2f bulletPos = this->sprite->getPosition();
	TD::ExplosiveTower* expTower = dynamic_cast<TD::ExplosiveTower*>(tower);
		
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		if (!(*enemies[i].get())->isSpawn())
			continue;

		sf::Vector2f enPos = (*enemies[i].get())->getSprite()->getPosition();
		float distance = sqrt((enPos.x - bulletPos.x) * (enPos.x - bulletPos.x) + (enPos.y - bulletPos.y) * (enPos.y - bulletPos.y));
		if (distance > expTower->getBulletRadius())
			continue;

		if ((int)(*enemies[i].get())->getConfig().life - (int)tower->config.damage < 0)
			(*enemies[i].get())->getConfig().life = 0;
		else
			(*enemies[i].get())->getConfig().life -= tower->config.damage;
		(*enemies[i].get())->hit();

		if ((*enemies[i].get())->getConfig().life == 0)
		{
			std::weak_ptr<TD::Enemy*> tmpWeakEnemy = enemies[i];
			std::shared_ptr<TD::Enemy*> tmpEnemy = tmpWeakEnemy.lock();

			player.updateScore((*enemies[i].get())->getConfig().gold);
			player.updateMoney((*enemies[i].get())->getConfig().gold);

			renderer.RemoveSprite(*(*enemies[i].get())->getSprite());
			std::vector<std::shared_ptr<TD::Enemy*>>::iterator enemyIt = std::find(enemies.begin(), enemies.end(), enemies[i]);
			if (enemyIt != enemies.end())
			{
				enemies.erase(enemyIt);
				delete (*tmpEnemy.get())->getSprite();
				delete *tmpEnemy.get();
			}
		}
	}
}

void TD::Bullet::makeStunDamage(Renderer& renderer, ITower* tower, std::vector<std::shared_ptr<TD::Enemy*>>& enemies, TD::Player& player, std::shared_ptr<TD::Enemy*> enemy)
{
	if ((int)(*enemy.get())->getConfig().life - (int)tower->config.damage < 0)
		(*enemy.get())->getConfig().life = 0;
	else
		(*enemy.get())->getConfig().life -= tower->config.damage;

	TD::StunTower* stun = dynamic_cast<TD::StunTower*>(tower);
	(*enemy.get())->setSlow(float(stun->slowCoef), stun->slowCoolDown);

	if ((*enemy.get())->getConfig().life == 0)
	{
		player.updateScore((*enemy.get())->getConfig().gold);
		player.updateMoney((*enemy.get())->getConfig().gold);

		renderer.RemoveSprite(*(*enemy.get())->getSprite());
		std::vector<std::shared_ptr<TD::Enemy*>>::iterator enemyIt = std::find(enemies.begin(), enemies.end(), enemy);
		if (enemyIt != enemies.end())
			enemies.erase(enemyIt);
		delete (*enemy.get())->getSprite();
		delete* enemy.get();
	}
}