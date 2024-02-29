#pragma once
#include "Tile.h"

#define PATH_COIN "Coins.png"

class Coin: public Tile
{
	int coins;


public:
	int GetCoins()
	{
		return coins;
	}

public:
	Coin(const int _coins, const string& _id, const Vector2f& _position);
public:
	void PickUpCoin();
};

