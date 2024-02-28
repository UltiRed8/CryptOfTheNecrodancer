#pragma once

class PlayerRessource
{
protected :
	int* money;
	int* diamonds;

public :
	int* GetMoney() const
	{
		return money;
	}
	int* GetDiamonds() const
	{
		return diamonds;
	}

public :
	PlayerRessource();
};

