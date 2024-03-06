#include "Ice.h"
#include "Macro.h"
#include "Animation.h"

Ice::Ice(const Vector2f& _position) : Placeable(STRING_ID("Ice"), "", _position)
{
	type = ET_WATER;
	zIndex = 0;

	// TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_WATER, Random(2, 0), Vector2i(26, 26));
	components.push_back(new AnimationComponent(this, {
		AnimationData("Ice", Vector2f(24, 24), 0, 3, 0.1f, true),
		}, "Ice", shape));
}
