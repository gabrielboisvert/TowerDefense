#pragma once
#include "Cell.h"
#include <vector>
#include <map>
#include <string>
#include <SFML/System/Vector2.hpp>
#include "Renderer.h"
#include "ITower.h"
#include "PathFinding.h"
#include "EnemyArmy.h"

namespace TD
{
	class EnemyArmy;
	class ITower;

	class GameMap {
	private:
		unsigned int cellSizeX;
		unsigned int cellSizeY;
		unsigned int startBudget;
		unsigned int defaultHQLife;
		
		std::vector<Sprite*> HQs;
		std::vector<TD::ITower*> towers;
		
		std::vector<std::vector<Cell*>> cells;
		
		ai::Graph graph;
		ai::Dijkstra dj;
		ai::Node::EdgeList path;

		std::vector<sf::Time> waveCounter;
	public:
		GameMap();

		bool buildFromFile(std::string& filename, Renderer& renderer, TD::EnemyArmy& army);
		
		bool findPath();

		template<typename T> T* addTower()
		{
			T* tower = new T();
			this->towers.push_back(tower);
			return tower;
		}

		void removeTower(ITower* tower);

		ITower* getTowerOnScreenPos(sf::Vector2i screenPos);

		const sf::Vector2i getPlayerHQPosition();

		const sf::Vector2i getSpawnPosition();

		const sf::Vector2f getCellSize() const;

		std::vector<std::vector<Cell*>>& getCells() { return this->cells; };

		ai::Node::EdgeList& getPath() { return this->path; };
		
		std::vector<TD::ITower*>& getTowers() { return this->towers; };

		unsigned int getStartBudget() { return this->startBudget; };

		unsigned int getdefaultHQLife() { return this->defaultHQLife; };

		std::vector<sf::Time>& getWaveTimer() { return this->waveCounter; };

		void fillSprite(Renderer& renderer);
		
		void clear();

		void reset();

		sf::Vector2i getMapPathIdx(unsigned int pathIndx);

		~GameMap();
	};
}