#include "Water.h"
#include "Macro.h"

Water::Water(const Vector2f& _position) : Pickable(0, PT_CUSTOM, STRING_ID("Water"), _position)
{
	type = ET_WATER;
}
