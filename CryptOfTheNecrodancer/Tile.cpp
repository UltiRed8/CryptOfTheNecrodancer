#include "Tile.h"
#include "Macro.h"
#include "Map.h"

#define PATH_A "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/FloorA.png"
#define PATH_B "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/FloorB.png"
#define PATH_C "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/FloorC.png"
#define PATH_D "Dungeons/" + Map::GetInstance().GetZoneFileName() + "/FloorD.png"

Tile::Tile(const Vector2f& _position, const EntityType& _type) : Placeable(STRING_ID("Tile"), PATH_A, _position)
{
	entities = vector<Entity*>();
	type = _type;
	if (_type == ET_SHADOW)
	{
		shape->setFillColor(Color::Black);
		shape->move(Vector2f(0.0f, -0.5f) * TILE_SIZE);
		zIndex = 4;
	}

	const Vector2i& _tilePosition = Vector2i(int(_position.x / TILE_SIZE.x), int(_position.y / TILE_SIZE.y));
	status = (_tilePosition.x + _tilePosition.y) % 2 == 0;
}

Tile::Tile(const string& _path, const Vector2f& _position, const EntityType& _type) : Placeable(STRING_ID("Tile"), _path, _position)
{
	entities = vector<Entity*>();
	type = _type;
	if (_type == ET_SHADOW)
	{
		shape->setFillColor(Color::Black);
		shape->move(Vector2f(0.0f, -0.5f) * TILE_SIZE);
		zIndex = 4;
	}

	const Vector2i& _tilePosition = Vector2i(int(_position.x / TILE_SIZE.x), int(_position.y / TILE_SIZE.y));
	status = (_tilePosition.x + _tilePosition.y) % 2 == 0;
}

void Tile::InvertAlpha(const bool _status, const bool _highlighted)
{
	if (_highlighted)
	{
		if (status)
		{
			TextureManager::GetInstance().Load(shape, _status ? PATH_A : PATH_C);
		}
		else
		{
			TextureManager::GetInstance().Load(shape, _status ? PATH_D : PATH_B);
		}
	}
	else
	{
		if (status)
		{
			TextureManager::GetInstance().Load(shape, _status ? PATH_A : PATH_B);
		}
		else
		{
			TextureManager::GetInstance().Load(shape, _status ? PATH_B : PATH_A);
		}
	}
}