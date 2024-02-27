#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"

Wall::Wall(const Vector2f& _position, const WallType& _type) : Entity(STRING_ID("Wall"), "", _position)
{
	type = _type;
	
	const int _randomValue = Random(15, 0);

	TextureManager::GetInstance().LoadFromTextureSheet(shape, GetPathWithType(type), _randomValue, Vector2i(24, 24));

}

void Wall::Update()
{
}

