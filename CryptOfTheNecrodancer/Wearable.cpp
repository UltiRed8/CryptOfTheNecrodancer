#include "Wearable.h"

Wearable::Wearable(ListWearable _type, const string& _id, const Vector2f& _position) : Items(_id, _position)
{
	type = _type;
}

void Wearable::BoostStats()
{

}

string Wearable::GetTexturePath()
{
	vector<string> _paths = {
		"",
		"",
	};

	return _paths[type];
}
