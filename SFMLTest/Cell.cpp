#include "pch.h"
#include "Cell.h"
#include <SFML\Graphics\Sprite.hpp>

using namespace TD;


void Cell::initCellImg(Renderer& renderer)
{
	for (unsigned int i = 0; i < 6; i++)
		renderer.LoadTextureFile(CELL_PATH[i]);
}

Cell::Cell(TerrainType type)
{
	this->terrain = type;
	this->sprite = nullptr;
}

TerrainType& TD::Cell::get_terrain()
{
	return this->terrain;
}

void TD::Cell::setSprite(Sprite& sprite)
{
	this->sprite = &sprite;
}

Sprite& TD::Cell::getSprite()
{
	return *this->sprite;
}

const char* TD::Cell::getTextureName() const
{
	switch (this->terrain) {
		case TerrainType::Hq: return CELL_PATH[0];
		case TerrainType::Spawn: return CELL_PATH[1];
		case TerrainType::Dirt: return CELL_PATH[2];
		case TerrainType::Grass: return CELL_PATH[3];
		case TerrainType::Sand: return CELL_PATH[4];
		case TerrainType::Water: return CELL_PATH[5];
	}
	return nullptr;
}

TD::Cell::~Cell() {}