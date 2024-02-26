#include "Tile.h"
#include "Macro.h"
Tile::Tile(const string& _tilePath, const Vector2f& _position) : Entity(STRING_ID("Tile"), _tilePath, _position)
{
	entities = vector<Entity*>();
	GetShape()->setOutlineColor(Color::Black);
	GetShape()->setOutlineThickness(-1.0f);
}

void Tile::Update()
{
}
