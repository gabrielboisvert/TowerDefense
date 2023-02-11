#include "pch.h"
#include "GameMap.h"
#include <fstream>
#include <sstream>
#include "EnemyConfig.h"
#include "ScoutEnemy.h"
#include "HealerEnemy.h"
#include "TankEnemy.h"

using namespace TD;

TD::GameMap::GameMap()
{
	this->cellSizeX = 0;
	this->cellSizeY = 0;
	this->startBudget = 0;
	this->defaultHQLife = 0;
}

bool TD::GameMap::buildFromFile(std::string& filename, Renderer& renderer, TD::EnemyArmy& army)
{
	std::ifstream ifi(filename.c_str());
	if (!ifi.is_open())
		return false;

	std::map<std::string, std::size_t> map;
	std::string line;
	unsigned int warmup = 0;
	while (std::getline(ifi, line))
	{
		std::size_t tab = line.find_first_of('\t');
		if (tab != std::string::npos)
			line.erase(tab);

		std::size_t multSpace = line.find("   ");
		if (multSpace != std::string::npos)
			line.erase(multSpace);

		std::size_t spaceComment = line.find(" #");
		if (spaceComment != std::string::npos)
			line.erase(spaceComment);

		std::size_t comment = line.find('#');
		if (comment != std::string::npos)
			line.erase(comment);

		if (line.empty())
			continue;

		std::istringstream is(line);

		std::string key;
		is >> key;
		if (key.empty() || line._Equal(key))
			continue;

		if (key._Equal("map"))
		{
			unsigned int column;
			is >> column;

			this->cellSizeX = renderer.getWindow()->getWindow().getSize().x / column;

			unsigned int row;
			is >> row;

			row += 1;

			this->cellSizeY = renderer.getWindow()->getWindow().getSize().y / row;

			for (unsigned int i = 1; i < row; i++)
			{
				std::getline(ifi, line);
				std::istringstream is(line);

				std::vector<Cell*> cells;
				for (unsigned int j = 0; j < column; j++)
				{
					unsigned int value;
					is >> value;


					Cell* cell = new Cell(static_cast<TerrainType>(value));
					cells.push_back(cell);

					sf::Vector2i vec2(j * this->cellSizeX, i * this->cellSizeY);
					if (cell->get_terrain() == TerrainType::Hq)
					{
						TD::Sprite& sprite = renderer.CreateSprite(*renderer.GetTexture(CELL_PATH[1]), vec2);
						cell->setSprite(sprite);
						sprite.setScale(this->cellSizeX / sprite.getGlobalBounds().width, this->cellSizeY / sprite.getGlobalBounds().height);

						
						Sprite& HQ = renderer.CreateSprite(*renderer.GetTexture(cell->getTextureName()), vec2, 1);
						HQ.setScale(this->cellSizeX / HQ.getGlobalBounds().width, this->cellSizeY / HQ.getGlobalBounds().height);
						this->HQs.push_back(&HQ);
					}
					else
					{
						TD::Sprite& sprite = renderer.CreateSprite(*renderer.GetTexture(cell->getTextureName()), vec2);
						cell->setSprite(sprite);
						sprite.setScale(this->cellSizeX / sprite.getGlobalBounds().width, this->cellSizeY / sprite.getGlobalBounds().height);
					}

				}
				this->cells.push_back(cells);
			}
			break;
		}
		else if (key._Equal("warmup"))
			is >> warmup;
		else if (key._Equal("budget"))
			is >> this->startBudget;
		else if (key._Equal("hq_init_life"))
			is >> this->defaultHQLife;
		else if (key._Equal("enemy"))
		{
			unsigned int enemyType;
			is >> enemyType;
			TD::EnemyType type = static_cast<TD::EnemyType>(enemyType);

			unsigned int delay;
			is >> delay;

			TD::Enemy* enemy = nullptr;
			if (type == TD::EnemyType::SCOUT)
				enemy = army.spawnEnemy<TD::ScoutEnemy>();
			else if (type == TD::EnemyType::TANK)
				enemy = army.spawnEnemy<TD::TankEnemy>();
			else if (type == TD::EnemyType::HEALER)
				enemy = army.spawnEnemy<TD::HealerEnemy>();

			if (enemy != nullptr)
				enemy->setSpawnTimer(sf::seconds(float(delay + warmup)));
		}	
		else if (key._Equal("wave"))
		{
			unsigned int delay;
			is >> delay;

			warmup += delay;

			this->waveCounter.push_back(sf::seconds(float(warmup)));
		}
	}

	ifi.close();
	return true;
}

bool TD::GameMap::findPath()
{
	this->graph = ai::Graph(this->cells);
	this->path.clear();
	this->dj.getPath().clear();
	
	if (!this->dj.pathFinder(this->graph))
		return false;
	
	this->path = dj.getPath();
	return true;
}

const sf::Vector2i TD::GameMap::getPlayerHQPosition()
{
	for (unsigned int i = 0; i < this->cells.size(); i++)
		for (unsigned int j = 0; j < this->cells[i].size(); j++)
		{
			if (this->cells[i][j]->get_terrain() == TerrainType::Hq)
				return sf::Vector2i(j * this->cellSizeX, i * this->cellSizeY);
		}
	return sf::Vector2i(0, 0);
}

const sf::Vector2i TD::GameMap::getSpawnPosition()
{
	for (unsigned int i = 0; i < this->cells.size(); i++)
		for (unsigned int j = 0; j < this->cells[i].size(); j++)
		{
			if (this->cells[i][j]->get_terrain() == TerrainType::Spawn)
				return sf::Vector2i(j * this->cellSizeX, (i+1) * this->cellSizeY);
		}

	return sf::Vector2i(0, 0);
}

void TD::GameMap::removeTower(ITower* tower)
{
	this->towers.erase(this->towers.end() - 1);
}

ITower* TD::GameMap::getTowerOnScreenPos(sf::Vector2i screenPos)
{
	for (unsigned int i = 0; i < this->towers.size(); i++)
		if (this->towers[i]->getSprite()->getGlobalBounds().contains((sf::Vector2f)screenPos))
			return this->towers[i];
	return nullptr;
}

const sf::Vector2f TD::GameMap::getCellSize() const
{
	return sf::Vector2f(float(this->cellSizeX), float(this->cellSizeY));
}

void TD::GameMap::fillSprite(Renderer& renderer)
{
	for (unsigned int i = 0; i < this->cells.size(); i++)
		for (unsigned int j = 0; j < this->cells[i].size(); j++)
			renderer.addSprite(this->cells[i][j]->getSprite());

	for (unsigned int i = 0; i < this->HQs.size(); i++)
		renderer.addSprite(*this->HQs[i], 1);

	for (unsigned int i = 0; i < this->towers.size(); i++)
		renderer.addSprite(*this->towers[i]->getSprite(), 2);
}

void GameMap::clear()
{
	this->cells.clear();
	this->towers.clear();
	this->HQs.clear();
	
	this->cellSizeX = 0;
	this->cellSizeY = 0;
	this->startBudget = 0;
	this->defaultHQLife = 0;
}

void GameMap::reset()
{
	std::vector<Sprite*> HQs;
	std::vector<TD::ITower*> towers;

	std::vector<std::vector<Cell*>> cells;

	for (unsigned int i = 0; i < this->HQs.size(); i++)
		delete this->HQs[i];
	this->HQs.clear();

	for (unsigned int i = 0; i < this->towers.size(); i++)
		delete this->towers[i];
	this->towers.clear();

	for (unsigned int i = 0; i < this->cells.size(); i++)
	{
		for (unsigned int j = 0; j < this->cells[i].size(); j++)
			delete this->cells[i][j];
		this->cells[i].clear();
	}
	this->cells.clear();

	this->waveCounter.clear();
}

sf::Vector2i TD::GameMap::getMapPathIdx(unsigned int pathIndx)
{
	unsigned int row = unsigned int(this->cells[0].size());
	return sf::Vector2i(pathIndx / row, pathIndx % row);
}

TD::GameMap::~GameMap()
{
	for (unsigned int i = 0; i < this->cells.size(); i++)
		for (unsigned int j = 0; j < this->cells[i].size(); j++)
			delete this->cells[i][j];

	for (unsigned int i = 0; i < this->towers.size(); i++)
		delete this->towers[i];
}
