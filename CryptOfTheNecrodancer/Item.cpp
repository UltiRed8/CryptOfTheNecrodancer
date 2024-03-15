#include "Item.h"
#include "TextureManager.h"
#include "Macro.h"
#include "TimerManager.h"
#include "SoundManager.h"
#include "Math.h"
#include "EntityManager.h"
#include "WindowManager.h"
#include "Player.h"
#include "Map.h"

#define PI 3.14159265358979323846f

#define PATH_SHADOW "Entities/Shadow.png"

#define SOUND_PICKUP_ARMOR "Assets/Sounds/sfx_pickup_armor.ogg"
#define SOUND_PICKUP_WEAPON "Assets/Sounds/sfx_pickup_weapon.ogg"
#define SOUND_SHOP_BUY "Assets/Sounds/sfx_pickup_purchase.ogg"
#define SOUND_SHOP_ERROR "Assets/Sounds/sfx_error_ST.ogg"

#define PATH_SAVE "Assets/Saved/PurchasedItems.txt"

Item::Item(const SlotType& _type, const string& _id, const Vector2f& _position, const bool _isInInventory) : Entity(_id, "", _position)
{
	id = 0;
	inShop = true;
	isPickable = false;
	visuals = new RectangleShape(TILE_SIZE);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);
	TextureManager::GetInstance().Load(shape, PATH_SHADOW);
	stats = ItemStats();
	stype = _type;
	type = ET_ITEM;
	zIndex = 3;
	isInInventory = _isInInventory;
	animationValue = (float)Random((const int)(floor(2.0f * PI)), 0);
	animationTimer = new Timer(STRING_ID("ItemAnimation"), [this]() {
		animationValue += 0.1f;
		visuals->setPosition(GetPosition() + Vector2f(0.0f, -0.5f - sin(animationValue) * 0.05f) * TILE_SIZE);
		int _opacity = (const int)(255 * ((1 - (sin(animationValue))) / 2.0f));
		_opacity = _opacity < 150 ? 150 : _opacity;
		shape->setFillColor(Color(255, 255, 255, _opacity));
		if (animationValue >= 2.0f * PI)
		{
			animationValue = 0.0f;
		}
	}, seconds(0.1f), -1, false, false);
	text = nullptr;
}

void Item::UpdateText()
{
	if (!inShop) return;
	if (Map::GetInstance().IsInLobby())
	{
		SetText(to_string(stats.diamondPrice));
	}
	else
	{
		if (inShop)
		{
			SetText(to_string(stats.goldPrice));
		}
	}
}

Item::~Item()
{
	delete animationTimer;
	if (text)
	{
		delete text;
	}
}

void Item::UpdateTexture()
{
	TextureManager::GetInstance().Load(visuals, GetTexturePath());
}

void Item::SaveItem()
{
	ofstream _out = ofstream(PATH_SAVE, ios_base::app);
	_out << id << endl;
}

bool Item::PickUp()
{
	Player* _player = (Player*)EntityManager::GetInstance().Get("Player");

	if (inShop)
	{
		if (Map::GetInstance().IsInLobby())
		{
			if (*_player->GetRessources()->GetDiamonds() >= stats.diamondPrice)
			{
				*_player->GetRessources()->GetDiamonds() -= stats.diamondPrice;
				SoundManager::GetInstance().Play(SOUND_SHOP_BUY);
				SaveItem();
				if (isPickable)
				{
					ExecuteCallback();
				}
				Destroy();
				return true;
			}
			else
			{
				SoundManager::GetInstance().Play(SOUND_SHOP_ERROR);
				WindowManager::GetInstance().Shake(Vector2f(*EntityManager::GetInstance().Get("Player")->GetComponent<MovementComponent>()->GetDirection()));
				return false;
			}
		}
		else
		{
			if (*_player->GetRessources()->GetMoney() >= stats.goldPrice)
			{
				*_player->GetRessources()->GetMoney() -= stats.goldPrice;
				SoundManager::GetInstance().Play(SOUND_SHOP_BUY);
				delete text;
				text = nullptr;
				inShop = false;
			}
			else
			{
				SoundManager::GetInstance().Play(SOUND_SHOP_ERROR);
				WindowManager::GetInstance().Shake(Vector2f(*EntityManager::GetInstance().Get("Player")->GetComponent<MovementComponent>()->GetDirection()));
				return false;
			}
		}
	}

	if (isPickable)
	{
		ExecuteCallback();
		Destroy();
		return true;
	}

	Inventory* _inventory = _player->GetInventory();
	Slot* _slot = _inventory->GetSlot(stype);

	if (stype == ST_FOOD_TOP && _slot->GetItem() != nullptr)
	{
		_slot = _inventory->GetSlot(ST_FOOD_DOWN);
		stype = ST_FOOD_DOWN;
	}

	if (Item* _item = _slot->GetItem())
	{
		_item->SetInInventory(false);
		_item->SetPosition(GetPosition());
		Map::GetInstance().GetGenerator()->AddItem(_item);
		if (_item->stype == ST_FOOD_DOWN)
		{
			_item->stype = ST_FOOD_TOP;
		}
	}

	int _slotID = (int)_slot->GetType();
	if (_slotID == 1)
	{
		SoundManager::GetInstance().Play(SOUND_PICKUP_WEAPON);
	}
	else if (_slotID >= 2 && _slotID <= 4)
	{
		SoundManager::GetInstance().Play(SOUND_PICKUP_ARMOR);
	}
	else
	{
		SoundManager::GetInstance().Play(SOUND_PICKUP_GENERAL);
	}

	_slot->SetItem(this);
	_slot->SetVisible();

	SetInInventory(true);

	if (_slot->GetType() == ST_ATTACK)
	{
		_player->UpdateDamageZone();
	}

	return true;
}

Armor::Armor(const ArmorType& _aType, const string& _id, const Vector2f& _position, const bool _isInInventory, const bool _inShop) : Item(GetSlotTypeWithArmorType(_aType), _id, _position, _isInInventory)
{
	inShop = _inShop;
	armorType = _aType;
	id = armorType + 200;
	stats = UpdateStat();
	UpdateTexture();
	if (armorType == AT_HEAD_MINERSCAP)
	{
		callback = [this]() {
			Vector2f _directions[] = {
				Vector2f(0.0f, 1.0f),
				Vector2f(0.0f, -1.0f),
				Vector2f(1.0f, 0.0f),
				Vector2f(-1.0f, 0.0f),
			};
			Player* _player = (Player*)EntityManager::GetInstance().Get("Player");
			if (_player)
			{
				for (const Vector2f& _direction : _directions)
				{
					Entity* _entity = Map::GetInstance().GetEntityAt(_player->GetPosition() + _direction * TILE_SIZE);
					if (_entity)
					{
						if (_entity->GetType() == ET_WALL)
						{
							if (Wall* _wall = (Wall*)_entity)
							{
								_wall->DestroyWall(2, true, false);
							}
						}
					}
				}
			}
		};
	}
	UpdateText();
}