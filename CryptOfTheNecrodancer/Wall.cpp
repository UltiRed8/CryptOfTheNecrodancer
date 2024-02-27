#include "Wall.h"
#include"Macro.h"
#include "TextureManager.h"

Wall::Wall(const Vector2f& _position, bool _isInvulnerable, const WallType& _type) :Entity(STRING_ID("Wall"), "", _position)
{
	type = _type;
	isInvulnerable = _isInvulnerable;
	if (isInvulnerable)
	{
		type = WT_INVULNERABLE;
	}
	const int _randomValue = Random(15, 0);

	TextureManager::GetInstance().LoadFromTextureSheet(shape, GetPathWithType(type), _randomValue, Vector2i(24, 24));

}

void Wall::Update()
{
}

