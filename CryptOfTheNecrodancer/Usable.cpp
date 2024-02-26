#include "Usable.h"

string Usable::GetTexturePath()
{
	vector<string> _paths = {
		"",
		"",
	};

	return _paths[type];
}
