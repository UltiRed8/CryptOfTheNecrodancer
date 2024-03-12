#include "Water.h"
#include "Macro.h"

Water::Water(const Vector2f& _position) : Tile("", _position, ET_WATER)
{
	type = ET_WATER;
	zIndex = 0;

	TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER, Random(2, 0), Vector2i(26, 26));
}