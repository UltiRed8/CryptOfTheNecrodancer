#pragma once

#include "EntityManager.h"
#include "Player.h"
#include "SoundManager.h"
#include "Items.h"

#define PATH_AXE "Items/Weapon/WeaponAxe.png"
#define PATH_AXE_TITANIUM "Items/Weapon/WeaponAxeTitanium.png"
#define PATH_BROADSWORD "Items/Weapon/WeaponBroadsword.png"
#define PATH_BROADSWORD_TITANIUM "Items/Weapon/WeaponBroadswordTitanium.png"
#define PATH_CAT "Items/Weapon/WeaponCat.png"
#define PATH_CAT_TITANIUM "Items/Weapon/WeaponCatTitanium.png"
#define PATH_CUTLASS "Items/Weapon/WeaponCutlass.png"
#define PATH_CUTLASS_TITANIUM "Items/Weapon/WeaponCutlassTitanium.png"
#define PATH_DAGGER "Items/Weapon/WeaponDagger.png"
#define PATH_DAGGER_JEWELED "Items/Weapon/WeaponDaggerJeweled.png"
#define PATH_DAGGER_TITANIUM "Items/Weapon/WeaponDaggerTitanium.png"
#define PATH_FLAIL "Items/Weapon/WeaponFlail.png"
#define PATH_FLAIL_TITANIUM "Items/Weapon/WeaponFlailTitanium.png"
#define PATH_HARP "Items/Weapon/WeaponHarp.png"
#define PATH_HARP_TITANIUM "Items/Weapon/WeaponHarpTitanium.png"
#define PATH_LONGSWORD "Items/Weapon/WeaponLongsword.png"
#define PATH_LONGSWORD_TITANIUM "Items/Weapon/WeaponLongswordTitanium.png"
#define PATH_RAPIER "Items/Weapon/WeaponRapier.png"
#define PATH_RAPIER_TITANIUM "Items/Weapon/WeaponRapierTitanium.png"
#define PATH_SPEAR "Items/Weapon/WeaponSpear.png"
#define PATH_SPEAR_TITANIUM "Items/Weapon/WeaponSpearTitanium.png"
#define PATH_STAFF "Items/Weapon/WeaponStaff.png"
#define PATH_STAFF_TITANIUM "Items/Weapon/WeaponStaffTitanium.png"
#define PATH_WARHAMMER "Items/Weapon/WeaponWarhammer.png"
#define PATH_WARHAMMER_TITANIUM "Items/Weapon/WeaponWarhammerTitanium.png"
#define PATH_WHIP "Items/Weapon/WeaponWhip.png"
#define PATH_WHIP_TITANIUM "Items/Weapon/WeaponWhipTitanium.png"

#define PATH_PICKAXE "Items/Shovel/Pickaxe.png"
#define PATH_SHOVEL "Items/Shovel/Shovel.png"
#define PATH_SHOVEL_COURAGE "Items/Shovel/ShovelCourage.png"
#define PATH_SHOVEL_CRYSTAL "Items/Shovel/ShovelCrystal.png"
#define PATH_SHOVEL_TITANIUM "Items/Shovel/ShovelTitanium.png"

#define PATH_HEAD_HELMET "Items/Head/Helmet.png"
#define PATH_HEAD_MINERSCAP "Items/Head/MinersCap.png"
#define PATH_BODY_CHAINMAIL "Items/Body/Chainmail.png"
#define PATH_BODY_LEATHERARMOR "Items/Body/LeatherArmor.png"
#define PATH_BODY_PLATEMAIL "Items/Body/Platemail.png"
#define PATH_BODY_HEAVYPLATE "Items/Body/Heavyplate.png"
#define PATH_FEET_BOOTSOFSTRENGTH "Items/Feet/BootsOfStrength.png"
#define PATH_FEET_HARGREAVES "Items/Feet/Hargreaves.png"

#define SOUND_COIN_PICKED_UP "Assets/Sounds/sfx_pickup_gold_01.ogg"
#define SOUND_DIAMOND_PICKED_UP "Assets/Sounds/sfx_pickup_diamond.ogg"
#define SOUND_PICKUP_GENERAL "Assets/Sounds/sfx_pickup_general_ST.ogg"

#define PATH_COIN "Entities/Coins.png"
#define PATH_DIAMOND "UI/Diamond.png"
#define PATH_HEART "UI/FullHeart.png"
#define PATH_SHADOW "Entities/Shadow.png"

#define FONT "Assets/Font/Font.ttf"

class Map;

struct InteractionData
{
	FloatRect rect;
	bool isRed;

public:
	InteractionData(const FloatRect& _rect, const bool _isRed)
	{
		rect = _rect;
		isRed = _isRed;
	}
};

struct ItemStats
{
	float damages;
	float defense;
	int digLevel;
	vector<InteractionData> attackRange;
	bool preventMovement;

public:
	ItemStats() = default;
	ItemStats(const float _damages, const float _defense, const int _digLevel, const vector<InteractionData>& _attackRange, const bool _preventMovement)
	{
		damages = _damages;
		defense = _defense;
		digLevel = _digLevel;
		attackRange = _attackRange;
		preventMovement = _preventMovement;
	}
};

class Item : public Entity
{
	RectangleShape* visuals;
	SlotType stype;
	Timer* animationTimer;
	float animationValue;
	bool isInInventory;
	Text* text;

protected:
	bool isPickable;
	ItemStats stats;
	function<void()> callback;

public:
	Item(const SlotType& _type, const string& _id, const Vector2f& _position,const bool _isInInventory = false);
	~Item();

protected:
	void UpdateTexture();

public:
	void SetText(const string& _text)
	{
		text = new Text();
		text->setString(_text);
		FontManager::GetInstance().Load(text, FONT);
		text->setCharacterSize(15);
		SetOriginCentered(text);
		text->setPosition(shape->getPosition() + Vector2f(0.5f, 0.5f) * TILE_SIZE);
	}
	void ExecuteCallback()
	{
		if (callback)
		{
			callback();
		}
	}
	ItemStats GetStats() const
	{
		return stats;
	}
	RectangleShape* GetVisuals() const
	{
		return visuals;
	}

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
		if (text)
		{
			_drawables.push_back(text);
		}
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
			PATH_AXE,
			PATH_AXE_TITANIUM,

			PATH_BROADSWORD,
			PATH_BROADSWORD_TITANIUM,

			PATH_CAT,
			PATH_CAT_TITANIUM,

			PATH_CUTLASS,
			PATH_CUTLASS_TITANIUM,

			PATH_DAGGER,
			PATH_DAGGER_JEWELED,
			PATH_DAGGER_TITANIUM,

			PATH_FLAIL,
			PATH_FLAIL_TITANIUM,

			PATH_HARP,
			PATH_HARP_TITANIUM,

			PATH_LONGSWORD,
			PATH_LONGSWORD_TITANIUM,
			PATH_RAPIER,
			PATH_RAPIER_TITANIUM,
			PATH_SPEAR,
			PATH_SPEAR_TITANIUM,
			PATH_STAFF,
			PATH_STAFF_TITANIUM,
			PATH_WARHAMMER,
			PATH_WARHAMMER_TITANIUM,
			PATH_WHIP,
			PATH_WHIP_TITANIUM,
		};
		return _pathes[weaponType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, -1.0f, 1.0f, 3.0f), false) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, -1.0f, 1.0f, 3.0f), false) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 1.0f, 3.0f), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 1.0f, 3.0f), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, false),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, false),

			ItemStats(1.0f, 0.5f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f), true) }, false),
			ItemStats(2.0f, 1.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f), true) }, false),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),
			ItemStats(5.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(-1.0f, -1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(-1.0f, 1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(-1.0f, 0.0f, 1.0f, 1.0f), false) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(-1.0f, -1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(-1.0f, 1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(-1.0f, 0.0f, 1.0f, 1.0f), false) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 1.0f, 1.0f), false) }, false),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 1.0f, 1.0f), false) }, false),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 20.0f, 1.0f ), false) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 20.0f, 1.0f ), false) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 2.0f, 3.0f), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 2.0f, 3.0f), true) }, true),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -2.0f, 1.0f, 5.0f), true) }, true),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -2.0f, 1.0f, 5.0f), true) }, true),
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
			PATH_SHOVEL_COURAGE,
			PATH_SHOVEL_CRYSTAL,
			PATH_SHOVEL_TITANIUM,
		};
		return _pathes[pickaxeType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			ItemStats(0.0f, 0.0f, 2, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, false),
			ItemStats(0.0f, 0.0f, 2, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, false),
		};
		return _values[pickaxeType];
	}
};

struct Armor : Item
{
	ArmorType armorType;

	public:
		Armor(const ArmorType& _aType, const string& _id, const Vector2f& _position, const bool _isInInventory = false);

public:
	ArmorType GetArmorType() const
	{
		return armorType;
	}
	SlotType GetSlotTypeWithArmorType(const ArmorType& _aType)
	{
		vector<SlotType> _tab;
		for (int _i = 0; _i < 2; _i++) _tab.push_back(ST_HEAD);
		for (int _i = 0; _i < 4; _i++) _tab.push_back(ST_BODY);
		for (int _i = 0; _i < 2; _i++) _tab.push_back(ST_FEET);

		return _tab[_aType];
	}
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_HEAD_HELMET,
			PATH_HEAD_MINERSCAP,
			PATH_BODY_CHAINMAIL,
			PATH_BODY_LEATHERARMOR,
			PATH_BODY_HEAVYPLATE,
			PATH_BODY_PLATEMAIL,
			PATH_FEET_BOOTSOFSTRENGTH,
			PATH_FEET_HARGREAVES,
		};
		return _pathes[armorType];
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 0.0f, 3, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 1.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 2.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 1.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false),
		};
		return _values[armorType];
	}
};

struct Pickable : Item
{
	PickableType pickableType;
	int amount;

public:
	Pickable(const PickableType& _pickableType, const string& _id, const Vector2f& _position, const int _amount = 1) : Item(ST_ATTACK, _id, _position, false)
	{
		pickableType = _pickableType;
		amount = _amount;
		InitCallback();
		UpdateTexture();
		zIndex = 3;
		isPickable = true;
		if (pickableType == PIT_COIN)
		{
			TextureManager::GetInstance().LoadFromTextureSheet(GetVisuals(), PATH_COIN, Random(3, 0), Vector2i(24, 24));
		}
	}

public:
	void InitCallback()
	{
		const function<void()> _callbacks[] = {
			[this]() { ((Player*)(EntityManager::GetInstance().Get("Player")))->GetRessources()->AddMoney(amount); SoundManager::GetInstance().Play(SOUND_COIN_PICKED_UP); },
			[this]() { ((Player*)(EntityManager::GetInstance().Get("Player")))->GetRessources()->AddDiamonds(amount); SoundManager::GetInstance().Play(SOUND_DIAMOND_PICKED_UP); },
			[this]() { ((Player*)(EntityManager::GetInstance().Get("Player")))->AddHeart(); SoundManager::GetInstance().Play(SOUND_PICKUP_GENERAL);},
		};
		callback = _callbacks[pickableType];
	}
	virtual ItemStats UpdateStat() override
	{
		return ItemStats();
	}
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_COIN,
			PATH_DIAMOND,
			PATH_HEART,
		};
		return _pathes[pickableType];
	}
};