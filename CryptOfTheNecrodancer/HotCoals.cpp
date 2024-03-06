#include "HotCoals.h"
#include "Macro.h"

HotCoals::HotCoals(const Vector2f& _position) : Placeable(STRING_ID("Ice"), "", _position)
{
	type = ET_ICE;
	zIndex = 0;

	components.push_back(new AnimationComponent(this, {
		AnimationData("Ice", Vector2f(26, 26), 0, 3, 0.1f, true),
		}, "Ice", shape));
}
