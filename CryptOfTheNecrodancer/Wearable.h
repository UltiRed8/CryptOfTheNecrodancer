#pragma once
#include "Items.h"

enum ListWearable
{
	MINERS_CAP, HELMET, // Headset
	LEATHER_ARMOR, CHAINMAIL, // Armor
	HARGREAVES, BOOTS_OF_STRENGTH // Boots
};

class Wearable : public Items
{
	ListWearable type;
public :
	Wearable(ListWearable _type, const string& _id, const Vector2f& _position);
public:
	void BoostStats(); //TODO A REVOIR
	virtual string GetTexturePath() override;
};

