#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"

Wall::Wall(const Vector2f& _position) :Entity(STRING_ID("Wall"), "", _position)
{
	const int _randomValue = Random(15, 0);
	TextureManager::GetInstance().LoadFromTextureSheet(shape, "Walls.png", _randomValue,Vector2i(24,48));

}

void Wall::Update()
{
}

