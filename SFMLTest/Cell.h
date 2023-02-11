#pragma once
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include "Renderer.h"

enum class TerrainType
{
	Hq = 0,
	Spawn = 1,
	Dirt = 2,
	Grass = 8,
	Sand = 16,
	Water = 99
};

static const char* CELL_PATH[] = { 
	"Assets/HQ.png",
	"Assets/textures/PNG/Default size/towerDefense_tile049.png",
	"Assets/textures/PNG/Default size/towerDefense_tile050.png",
	"Assets/textures/PNG/Default size/towerDefense_tile024.png",
	"Assets/textures/PNG/Default size/towerDefense_tile193.png",
	"Assets/textures/PNG/Default size/towerDefense_tile172.png"
};


namespace TD
{
	class Cell
	{
	private:
		TerrainType terrain;
		Sprite* sprite;
		
	public:
		static void initCellImg(Renderer& renderer);
		
		Cell(TerrainType type);

		TerrainType& get_terrain();

		const char* getTextureName() const;

		void setSprite(Sprite& sprite);

		Sprite& getSprite();

		~Cell();
	};
}