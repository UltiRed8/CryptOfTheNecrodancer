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
	void AddDiamonds(const int _diamonds = 1)
	{
		*diamonds += _diamonds;
	}
	void AddMoney(const int _money)
	{
		*money += _money;
	}

public :
	PlayerRessource();
};

