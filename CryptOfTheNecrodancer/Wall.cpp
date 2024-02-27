#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"

Wall::Wall(const Vector2f& _position,bool _isInvulnerable) :Entity(STRING_ID("Wall"), "", _position)
{

	isInvulnerable = _isInvulnerable;
	const int _randomValue = Random(15, 0);
	string _path = isInvulnerable ? "Bedrock.png" : "Walls.png";

	TextureManager::GetInstance().LoadFromTextureSheet(shape, _path, _randomValue, Vector2i(24, 24));

}

void Wall::Update()
{
}

