#pragma once
#include "Living.h"

class NPC : public Living
{
public:
	NPC(const float _maxHp,const float _maxDamages,const string& _id, const string& _path, const Vector2f& _position);
	NPC(const string& _path, const string& _id, const Vector2f& _position);
};

