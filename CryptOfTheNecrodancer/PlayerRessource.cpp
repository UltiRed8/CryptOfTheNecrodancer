#include "PlayerRessource.h"

PlayerRessource::PlayerRessource()
{
	money = new int(0);
	diamonds = new int(3);
}

PlayerRessource::~PlayerRessource()
{
	delete money;
	delete diamonds;
}
