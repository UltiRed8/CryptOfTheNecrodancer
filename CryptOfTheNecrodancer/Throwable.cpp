#include "Throwable.h"

void Throwable::Throw(const Vector2f& _distance, const float& _damageDeal)
{
	//if (_distance.x == 0 && _distance.y == 0) return;
	//if ennemy : vie ennemy -= _damageDeal
}

string Throwable::GetTexturePath()
{
	vector<string> _paths = {
		"",
		"",
	};

	return _paths[type];
}
