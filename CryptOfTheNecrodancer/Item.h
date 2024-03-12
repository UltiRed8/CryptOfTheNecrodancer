#pragma once
#include "Entity.h"
#include "Items.h"

#define PATH_BROADSWORD "Items/Weapon/Broadsword.png"
#define PATH_DAGGER "Items/Weapon/Dagger.png"
#define PATH_SPEAR "Items/Weapon/Spear.png"
#define PATH_STAFF "Items/Weapon/Staff.png"

#define PATH_PICKAXE "Items/Shovel/Pickaxe.png"
#define PATH_SHOVEL "Items/Shovel/Shovel.png"

#define PATH_HEAD_HELMET "Items/Head/Helmet.png"
#define PATH_HEAD_MINERSCAP "Items/Head/MinersCap.png"
#define PATH_BODY_CHAINMAIL "Items/Body/Chainmail.png"
#define PATH_BODY_LEATHERARMOR "Items/Body/LeatherArmor.png"
#define PATH_FEET_BOOTSOFSTRENGTH "Items/Feet/BootsOfStrength.png"

#define PATH_FEET_HARGREAVES "Items/Feet/Hargreaves.png"

struct ItemStats
{
	float damages;
	float defense;
	int digLevel;
};

class Item : public Entity
{
	RectangleShape* visuals;
	SlotType stype;
	Timer* animationTimer;
	float animationValue;
	bool isInInventory;

protected:
	ItemStats stats;

public:
	Item(const SlotType& _type, const string& _id, const Vector2f& _position,const bool _isInInventory = false);
	~Item();

protected :
	void UpdateTexture();

public :

	bool IsInInventory() const
	{
		return isInInventory;
	}

	void SetInInventory(bool _isInInventory)
	{
		isInInventory = _isInInventory;
	}

	void SetPosition(const Vector2f& _position)
	{
		shape->setPosition(_position);
	}

	virtual string GetTexturePath() = 0;
	virtual vector<Drawable*> GetDrawables() override
	{
		if (isInInventory)
		{
			return vector<Drawable*>();
		}
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		_drawables.push_back(visuals);
		return _drawables;
	}
	virtual ItemStats UpdateStat() = 0;

	virtual void PickUp();
};

struct Weapon : Item
{
	WeaponType weaponType;

public:
	Weapon(const WeaponType& _wType, const string& _id, const Vector2f& _position,const bool _isInInventory = false) : Item(ST_ATTACK, _id, _position, _isInInventory)
	{
		weaponType = _wType;
		stats = UpdateStat();
		UpdateTexture();
	}

public:
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_BROADSWORD,
			PATH_DAGGER,
			PATH_SPEAR,
			PATH_STAFF,
		};
		return _pathes[weaponType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			{ 1.0f, 0.0f, 0 },
			{ 1.0f, 0.0f, 0 },
			{ 1.0f, 0.0f, 0 },
			{ 1.0f, 0.0f, 0 },
		};
		return _values[weaponType];
	}
};

struct Pickaxe : Item
{
	PickaxeType pickaxeType;

public:
	Pickaxe(const PickaxeType& _pType, const string& _id, const Vector2f& _position,const bool _isInInventory = false) : Item(ST_SHOVEL, _id, _position,_isInInventory)
	{
		pickaxeType = _pType;
		stats = UpdateStat();
		UpdateTexture();
	}

public:
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_PICKAXE,
			PATH_SHOVEL,
		};
		return _pathes[pickaxeType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			{ 0.0f, 0.0f, 2 },
			{ 0.0f, 0.0f, 1 },
		};
		return _values[pickaxeType];
	}
};

struct Armor : Item
{
	ArmorType armorType;

	public:
	Armor(const ArmorType& _aType,  const string& _id, const Vector2f& _position,const bool _isInInventory = false) : Item(GetSlotTypeWithArmorType(_aType), _id, _position, _isInInventory)
	{
		armorType = _aType;
		stats = UpdateStat();
		UpdateTexture();
	}

public:
	SlotType GetSlotTypeWithArmorType(const ArmorType& _aType)
	{
		SlotType _tab[] =
		{
			ST_HEAD,
			ST_BODY,
			ST_FEET,
		};

		return _tab[(int)floor(_aType / 2.0f)];
	}
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_HEAD_HELMET,
			PATH_HEAD_MINERSCAP,
			PATH_BODY_CHAINMAIL,
			PATH_BODY_LEATHERARMOR,
			PATH_FEET_BOOTSOFSTRENGTH,
			PATH_FEET_HARGREAVES,
		};
		return _pathes[armorType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			{ 0.0f, 0.5f, 0 },
			{ 0.0f, 0.0f, 3 },
			{ 0.0f, 1.0f, 0 },
			{ 0.0f, 0.5f, 0 },
			{ 1.0f, 0.0f, 0 },
			{ 0.0f, 0.5f, 0 },
		};
		return _values[armorType];
	}
};