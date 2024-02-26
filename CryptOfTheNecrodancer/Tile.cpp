#include "Tile.h"

Tile::Tile(const string& _id, const string& _tilePath, const Vector2f& _position) : Entity(_id,_tilePath, _position)
{
	entities = vector<Entity*>();
}

void Tile::Update()
{
}
