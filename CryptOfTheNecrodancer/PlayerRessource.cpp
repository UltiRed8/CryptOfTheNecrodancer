#include "PlayerRessource.h"
#include <fstream>

PlayerRessource::PlayerRessource()
{
	money = new int(0);

	diamonds = new int(0);

	ifstream _stream = ifstream("Assets/Saved/PlayerStats.txt");
	if (_stream)
	{
		string _line;
		getline(_stream, _line);
		*diamonds = stoi(_line);
	}
}

PlayerRessource::~PlayerRessource()
{
	delete money;
	delete diamonds;
}
