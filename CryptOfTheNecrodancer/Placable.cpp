#include "Placable.h"
#include "Macro.h"

Placable::Placable(const string& _id, const string& _path, const Vector2f& pos) : Entity(_id, _path, pos)
{
	zIndex = 0;
}
