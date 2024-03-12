#include "Item.h"
#include "TextureManager.h"
#include "Macro.h"
#include "TimerManager.h"
#include "Math.h"
#include "EntityManager.h"
#include "Player.h"
//TODO DIRE A AXEL QUE C'EST UN ENORME CONNARD AVEC SON PTN DE BLOOM 
#define PATH_SHADOW "Entities/Shadow.png"

#define PI 3.14159265358979323846f

Item::Item(const SlotType& _type, const string& _id, const Vector2f& _position, const bool _isInInventory) : Entity(_id, "", _position)
{
	visuals = new RectangleShape(TILE_SIZE);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);
	TextureManager::GetInstance().Load(shape, PATH_SHADOW);
	stats = ItemStats();
	stype = _type;
	type = ET_ITEM;
	zIndex = 1;
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
	}, seconds(0.1f), -1, false);
}

Item::~Item()
{
	animationTimer->Pause();
	animationTimer->Destroy();
}

void Item::UpdateTexture()
{
	TextureManager::GetInstance().Load(visuals, GetTexturePath());
}

void Item::PickUp()
{
	Player* _player = (Player*)EntityManager::GetInstance().Get("Player");
	Inventory* _inventory = _player->GetInventory();
	Slot* _slot = _inventory->GetSlot(stype);

	if (_slot->GetItem())
	{
		Item* _item = _slot->GetItem();
		_item->SetInInventory(false);
		_item->SetPosition(_player->GetPosition() + Vector2f(0.0f, -0.5f) * TILE_SIZE);
	}
	_slot->SetTexture(GetTexturePath());
	_slot->SetItem(this);

	this->Destroy();
}
