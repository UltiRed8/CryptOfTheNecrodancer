#include "NPC.h"

NPC::NPC(const float _maxHp, const float _maxDamages, const string& _id, const string& _path, const Vector2f& _position)
	: Living(_maxHp, _maxDamages, _path, _id, _position)
{
	zIndex = 1;
}

NPC::NPC(const string& _path, const string& _id, const Vector2f& _position)
	: Living(_path, _id, _position)
{
	zIndex = 1;
}
