#include "pch.h"
#include "EnemyArmy.h"
#include "HealerEnemy.h"


TD::EnemyArmy::EnemyArmy()
{
	this->map = nullptr;
	this->player = nullptr;
	this->renderer = nullptr;
	this->waveText = nullptr;
	this->wave = 0;
}

void TD::EnemyArmy::removeEnemy(Enemy& en)
{
	for (unsigned int i = 0; i < this->enemies.size(); i++)
		if (*this->enemies[i].get() == &en)
			this->enemies.erase(this->enemies.begin() + i);
}

void TD::EnemyArmy::clearAllEnemy()
{
	this->enemies.clear();
}

void TD::EnemyArmy::reset()
{
	for (unsigned int i = 0; i < this->enemies.size(); i++)
		delete *this->enemies[i].get();
	this->enemies.clear();

	this->wave = 0;
}

std::vector<std::shared_ptr<TD::Enemy*>>& TD::EnemyArmy::getArmy()
{
	return this->enemies;
}

void TD::EnemyArmy::updateWave()
{
	this->wave++;

	if (this->waveText != nullptr)
		this->waveText->setString("Wave " + std::to_string(this->wave));
}

void TD::EnemyArmy::setWaveText(sf::Text* txt)
{
	if (txt == nullptr)
		return;

	this->waveText = txt;
}

void TD::EnemyArmy::initSprites(TD::GameMap& map, TD::Renderer& renderer, TD::Player& player)
{
	sf::Vector2i spawnPoint = map.getSpawnPosition();
	sf::Vector2f size = map.getCellSize();
	this->map = &map;
	this->player = &player;
	this->renderer = &renderer;

	for (unsigned int i = 0; i < this->enemies.size(); i++)
	{
		TD::Enemy* enemy = *this->enemies[i].get();
		
		TD::Sprite* sp = nullptr;
		if (TD::EnemyType::SCOUT == enemy->getType())
			sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[0]), spawnPoint, 1);
		else if (TD::EnemyType::TANK == enemy->getType())
			sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[1]), spawnPoint, 1);	
		else if (TD::EnemyType::HEALER == enemy->getType())
			sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[2]), spawnPoint, 1);
		
		if (sp != nullptr)
		{
			sp->setScale(size.x / sp->getGlobalBounds().width, size.y / sp->getGlobalBounds().height);
			
			sp->setOrigin(sp->getLocalBounds().width / 2, sp->getLocalBounds().height / 2);
			sp->move(sp->getGlobalBounds().width / 2, sp->getGlobalBounds().height / 2);
		}
		enemy->setSprite(sp);

		if (TD::EnemyType::HEALER == enemy->getType())
		{
			TD::HealerEnemy* healer = dynamic_cast<TD::HealerEnemy*>(enemy);
			healer->setRangeHeal();
		}

		enemy->spawn();
	}
}

void TD::EnemyArmy::initSprite(TD::GameMap& map, TD::Renderer& renderer, TD::Player& player, unsigned int idx)
{
	sf::Vector2i spawnPoint = map.getSpawnPosition();
	sf::Vector2f size = map.getCellSize();
	this->map = &map;
	this->player = &player;
	this->renderer = &renderer;

	TD::Enemy* enemy = *this->enemies[idx].get();

	TD::Sprite* sp = nullptr;
	if (TD::EnemyType::SCOUT == enemy->getType())
		sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[0]), spawnPoint, 1);
	else if (TD::EnemyType::TANK == enemy->getType())
		sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[1]), spawnPoint, 1);
	else if (TD::EnemyType::HEALER == enemy->getType())
		sp = &renderer.CreateSprite(*renderer.GetTexture(ENEMY_PATH[2]), spawnPoint, 1);

	if (sp != nullptr)
	{
		sp->setScale(size.x / sp->getGlobalBounds().width, size.y / sp->getGlobalBounds().height);

		sp->setOrigin(sp->getLocalBounds().width / 2, sp->getLocalBounds().height / 2);
		sp->move(sp->getGlobalBounds().width / 2, sp->getGlobalBounds().height / 2);
	}
	enemy->setSprite(sp);

	if (TD::EnemyType::HEALER == enemy->getType())
	{
		TD::HealerEnemy* healer = dynamic_cast<TD::HealerEnemy*>(enemy);
		healer->setRangeHeal();
	}

	enemy->spawn();
}

void TD::EnemyArmy::fillSprite(TD::Renderer& renderer)
{
	for (unsigned int i = 0; i < this->enemies.size(); i++)
		renderer.addSprite(*(*enemies[i].get())->getSprite(), 1);
}

TD::EnemyArmy::~EnemyArmy()
{
	for (unsigned int i = 0; i < this->enemies.size(); i++)
		delete *this->enemies[i].get();
	this->enemies.clear();
}
