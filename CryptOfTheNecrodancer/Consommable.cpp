#include "Consommable.h"

void Consommable::Heal(float _healAmount)
{
	//si consommable consommer
	// count--
	// vie du player += _healAmount
	//if (count <= 0) return;
}

string Consommable::GetTexturePath()
{
	vector<string> _paths = {
		"",
		"",
	};

	return _paths[type];
}
