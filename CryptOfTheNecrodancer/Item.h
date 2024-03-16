#pragma once

#include "EntityManager.h"
#include "Player.h"
#include "SoundManager.h"
#include "Items.h"
#include "Bomb.h"

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

#define PATH_FOOD_APPLE "Items/Item/Apple.png"
#define PATH_FOOD_CARROT "Items/Item/Carrot.png"
#define PATH_FOOD_CHEESE "Items/Item/Cheese.png"
#define PATH_FOOD_COOKIE "Items/Item/Cookie.png"
#define PATH_FOOD_DRUMSTICK "Items/Item/Drumstick.png"
#define PATH_FOOD_HAM "Items/Item/Ham.png"

#define SOUND_COIN_PICKED_UP "Assets/Sounds/sfx_pickup_gold_01.ogg"
#define SOUND_DIAMOND_PICKED_UP "Assets/Sounds/sfx_pickup_diamond.ogg"
#define SOUND_PICKUP_GENERAL "Assets/Sounds/sfx_pickup_general_ST.ogg"
#define SOUND_FOOD_USED "Assets/Sounds/sfx_item_food.ogg"

#define PATH_COIN "Entities/Coins.png"
#define PATH_DIAMOND "UI/Diamond.png"
#define PATH_HEART "UI/FullHeart.png"
#define PATH_SHADOW "Entities/Shadow.png"

#define PATH_BOMBITEM "Items/Bomb/Bomb.png"

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
	int diamondPrice;
	int goldPrice;
	string swipePath;
	int swipeAmount;
	bool swipeRotation;

public:
	ItemStats() = default;
	ItemStats(const float _damages, const float _defense, const int _digLevel, const vector<InteractionData>& _attackRange, const bool _preventMovement, const int _diamondPrice, const int _goldPrice, const string& _swipePath, const int _swipeAmount, const bool _swipeRotation = true)
	{
		swipeRotation = _swipeRotation;
		diamondPrice = _diamondPrice;
		goldPrice = _goldPrice;
		damages = _damages;
		defense = _defense;
		digLevel = _digLevel;
		attackRange = _attackRange;
		preventMovement = _preventMovement;
		swipePath = _swipePath;
		swipeAmount = _swipeAmount;
	}
};

class Item : public Entity
{
	RectangleShape* visuals;
	Timer* animationTimer;
	float animationValue;
	bool isInInventory;
	Text* text;

protected:
	int id;
	bool inShop;
	SlotType stype;
	bool isPickable;
	ItemStats stats;
	function<void()> callback;

public:
	Timer* GetAnimation() const
	{
		return animationTimer;
	}

public:
	Item(const SlotType& _type, const string& _id, const Vector2f& _position,const bool _isInInventory = false);
	~Item();

protected:
	void UpdateTexture();

public:
	void SaveItem();
	void UpdateText();
	SlotType GetSlotType() const
	{
		return stype;
	}
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
	virtual ItemStats UpdateStat() { return ItemStats(); };

	virtual bool PickUp();
};

struct Weapon : Item
{
	WeaponType weaponType;

public:
	Weapon(const WeaponType& _wType, const string& _id, const Vector2f& _position, const bool _isInInventory = false, const bool _inShop = true) : Item(ST_ATTACK, _id, _position, _isInInventory)
	{
		inShop = _inShop;
		weaponType = _wType;
		id = weaponType;
		stats = UpdateStat();
		UpdateTexture();
		UpdateText();
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
			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, -1.0f, 1.0f, 3.0f), false) }, true, 2, 50, "swipe_axe.png", 3),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, -1.0f, 1.0f, 3.0f), false) }, true, 3, 80, "swipe_axe.png", 3),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 1.0f, 3.0f), true) }, true, 1, 40, "swipe_broadsword.png", 3),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 1.0f, 3.0f), true) }, true, 2, 60, "swipe_broadsword_titanium.png", 3),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, false, 3, 60, "swipe_cat.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, false, 5, 90, "swipe_cat.png", 4),

			ItemStats(1.0f, 0.5f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f), true) }, false, 3, 60, "swipe_dagger.png", 3),
			ItemStats(2.0f, 1.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f), true) }, false, 5, 100, "swipe_dagger_titanium.png", 3),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 0, 10, "swipe_dagger.png", 3),
			ItemStats(5.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 8, 200, "swipe_dagger_jeweled.png", 3),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 2, 80, "swipe_dagger_titanium.png", 3),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, true, 3, 50, "swipe_flail.png", 5),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, -1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(0.0f, 1.0f, 2.0f, 1.0f), true), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true) }, true, 4, 80, "swipe_flail.png", 5),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(-1.0f, -1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(-1.0f, 1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(-1.0f, 0.0f, 1.0f, 1.0f), false) }, true, 5, 80, "swipe_harp.png", 5, false),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(-1.0f, -1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(-1.0f, 1.0f, 3.0f, 1.0f), false), InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(-1.0f, 0.0f, 1.0f, 1.0f), false) }, true, 6, 100, "swipe_harp.png", 5, false),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true, 3, 60, "swipe_longsword.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true, 4, 80, "swipe_longsword_titanium.png", 4),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 1.0f, 1.0f), false) }, false, 5, 80, "swipe_rapier.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 1.0f, 1.0f), false) }, false, 6, 120, "swipe_rapier_titanium.png", 4),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true, 2, 50, "swipe_dagger.png", 3),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 2.0f, 1.0f ), true) }, true, 3, 75, "swipe_dagger.png", 3),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 20.0f, 1.0f ), false) }, true, 4, 60, "swipe_staff.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f), true), InteractionData(FloatRect(2.0f, 0.0f, 20.0f, 1.0f ), false) }, true, 5, 90, "swipe_staff.png", 4),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 2.0f, 3.0f), true) }, true, 4, 60, "swipe_warhammer.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -1.0f, 2.0f, 3.0f), true) }, true, 5, 90, "swipe_warhammer.png", 4),

			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -2.0f, 1.0f, 5.0f), true) }, true, 4, 60, "swipe_whip.png", 4),
			ItemStats(2.0f, 0.0f, 0, { InteractionData(FloatRect(1.0f, -2.0f, 1.0f, 5.0f), true) }, true, 5, 90, "swipe_whip.png", 4),
		};
		return _values[weaponType];
	}
};

struct Pickaxe : Item
{
	PickaxeType pickaxeType;

public:
	Pickaxe(const PickaxeType& _pType, const string& _id, const Vector2f& _position, const bool _isInInventory = false, const bool _inShop = true) : Item(ST_SHOVEL, _id, _position, _isInInventory)
	{
		inShop = _inShop;
		pickaxeType = _pType;
		id = pickaxeType + 100;
		stats = UpdateStat();
		UpdateTexture();
		UpdateText();
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
			ItemStats(0.0f, 0.0f, 2, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 2, 60, "Pickaxe.png", 1),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 0, 10, "Shovel.png", 1),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, false, 1, 40, "ShovelCourage.png", 1),
			ItemStats(0.0f, 0.0f, 2, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, true, 3, 45, "ShovelCrystal.png", 1),
			ItemStats(0.0f, 0.0f, 1, { InteractionData(FloatRect(1.0f, 0.0f, 1.0f, 1.0f ), true) }, false, 2, 55, "ShovelTitanium.png", 1),
		};
		return _values[pickaxeType];
	}
};

struct Armor : Item
{
	ArmorType armorType;

	public:
		Armor(const ArmorType& _aType, const string& _id, const Vector2f& _position, const bool _isInInventory = false, const bool _inShop = true);

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
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 1, 50, "", 1),
			ItemStats(0.0f, 0.0f, 3, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 4, 120, "", 1),
			ItemStats(0.0f, 1.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 2, 75, "", 1),
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 1, 40, "", 1),
			ItemStats(0.0f, 2.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 4, 110, "", 1),
			ItemStats(0.0f, 1.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 3, 90, "", 1),
			ItemStats(1.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 1, 60, "", 1),
			ItemStats(0.0f, 0.5f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f ), true) }, false, 2, 80, "", 1),
		};
		return _values[armorType];
	}
};

struct Pickable : Item
{
	PickableType pickableType;
	int amount;

public:
	Pickable(const PickableType& _pickableType, const string& _id, const Vector2f& _position, const int _amount = 1) : Item(ST_NONE, _id, _position, false)
	{
		pickableType = _pickableType;
		amount = _amount;
		InitCallback();
		stats = UpdateStat();
		UpdateTexture();
		zIndex = 3;
		isPickable = true;
		if (pickableType == PIT_COIN)
		{
			TextureManager::GetInstance().LoadFromTextureSheet(GetVisuals(), PATH_COIN, Random(3, 0), Vector2i(24, 24));
		}
		inShop = pickableType == PIT_HEART ? true : false;
		UpdateText();
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
		const ItemStats _values[] = {
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 0, 0, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 0, 0, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 3, 0, "", 1),
		};
		return _values[pickableType];
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

struct BombItem : Item
{
public:
	void InitCallback()
	{
		callback = [&]() {
			Player* _player = (Player*)(EntityManager::GetInstance().Get("Player"));
			new Bomb(_player->GetPosition());
			_player->GetInventory()->GetSlot(ST_BOMB)->SetItem(nullptr);
			Destroy();
		};
	}

	BombItem(const Vector2f& _position, const bool _inInventory) : Item(ST_BOMB, STRING_ID("Bomb"), _position, _inInventory)
	{
		inShop = false;
		isPickable = false;
		UpdateTexture();
		InitCallback();
		UpdateText();
	}

	virtual string GetTexturePath() override
	{
		return PATH_BOMBITEM;
	}
};

struct Consomable : Item
{
	ConsomableType consomableType;
	int regen;

public:
	Consomable(const ConsomableType& _consomableType, const string& _id, const Vector2f& _position, const bool _inShop = true) : Item(ST_FOOD_TOP, _id, _position, false)
	{
		inShop = _inShop;
		consomableType = _consomableType;
		id = consomableType + 300;
		InitRegen();
		InitCallback();
		UpdateTexture();
		stats = UpdateStat();
		UpdateText();
	}

public:
	void InitRegen()
	{
		const int _values[] = {
			100, 100, 200, 50, 300, 400
		};

		regen = _values[consomableType];
	}
	void InitCallback()
	{
		callback = [this]() {
			Player* _player = (Player*)(EntityManager::GetInstance().Get("Player"));
			_player->GetComponent<LifeComponent>()->ChangeHealth((const float)regen);
			SoundManager::GetInstance().Play(SOUND_FOOD_USED);
			_player->GetInventory()->GetSlot(stype)->SetItem(nullptr);
			Destroy();
		};
	}
	virtual ItemStats UpdateStat() override
	{
		const ItemStats _values[] = {
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 1, 30, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 1, 30, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 2, 60, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 1, 15, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 3, 90, "", 1),
			ItemStats(0.0f, 0.0f, 0, { InteractionData(FloatRect(0.0f, 0.0f, 0.0f, 0.0f), true) }, false, 4, 120, "", 1),
		};
		return _values[consomableType];
	}
	virtual string GetTexturePath() override
	{
		const string _pathes[] = {
			PATH_FOOD_APPLE,
			PATH_FOOD_CARROT,
			PATH_FOOD_CHEESE,
			PATH_FOOD_COOKIE,
			PATH_FOOD_DRUMSTICK,
			PATH_FOOD_HAM,
		};
		return _pathes[consomableType];
	}
};