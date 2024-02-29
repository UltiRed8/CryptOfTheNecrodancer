#pragma once
#include "MenuManager.h"

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
	void SetDiamonds(int _diamonds)
	{
		*diamonds = _diamonds;
	}
	void SetMoney(int _money)
	{
		*money = _money;
	}

public :
	PlayerRessource();
	~PlayerRessource();
};

