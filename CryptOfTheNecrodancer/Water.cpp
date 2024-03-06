#include "Water.h"
#include "Macro.h"

Water::Water(const Vector2f& _position) : Placeable(STRING_ID("Water"), "", _position)
{
	type = ET_WATER;
	zIndex = 0;

	TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER, Random(2, 0), Vector2i(26, 26));
}
