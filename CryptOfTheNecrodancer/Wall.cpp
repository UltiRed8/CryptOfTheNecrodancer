#include "Wall.h"
#include"Macro.h"
Wall::Wall(const string& _wallPath, const Vector2f& _position) :Entity(STRING_ID("Wall"), _wallPath, _position)
{
}

void Wall::Update()
{
}
