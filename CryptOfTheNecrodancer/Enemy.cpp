#include "Enemy.h"

Enemy::Enemy(const string& _id,const string& _path,const Vector2f& _position):Entity(_id,_path,_position)
{
	indexPattern = 0;
	zIndex = 2;
}

