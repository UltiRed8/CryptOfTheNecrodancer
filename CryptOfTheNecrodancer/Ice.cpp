#include "Ice.h"
#include "Macro.h"
#include "Animation.h"
#include "AnimationComponent.h"

Ice::Ice(const Vector2f& _position) : Placeable(STRING_ID("Ice"), PATH_ICE, _position)
{
	type = ET_ICE;
	zIndex = 0;

	components.push_back(new AnimationComponent(this, {
		AnimationData("Ice", Vector2f(26, 26), 0, 2, 0.1f, true),
		}, "Ice", shape));
}
