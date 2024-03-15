#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Living.h"
#include "LifeComponent.h"
#include <SFML/Audio.hpp>
#include "MovementComponent.h"
#include "CameraManager.h"
using namespace sf;

struct AttackZone
{
	Vector2f offset;
	Shape* shape;
	bool isRed;

public:
	AttackZone(const Vector2f& _offset, Shape* _shape, const bool _isRed)
	{
		offset = _offset;
		shape = _shape;
		isRed = _isRed;
	}
	~AttackZone()
	{
		delete shape;
	}
};

class Player : public Living
{
	RectangleShape* visuals;
	Inventory* inventory;
	PlayerRessource* ressources;
	int* chainMultiplier;
	bool alreadyMoved;
	Menu* life;
	bool isConfuse;
	bool isStun;
	int heartIndex;
	bool pickupCooldown;
	vector<AttackZone*> damageZone;
	bool shouldSave;

public:
	bool GetPickupCooldown() const
	{
		return pickupCooldown;
	}
	int GetDigLevel() const;
	Inventory* GetInventory() const
	{
		return inventory;
	}
	bool GetPressingKeys()
	{
		return alreadyMoved;
	}
	bool GetIsStun() const
	{
		return isStun;
	}
	void SetIsStun(const bool _isStun = true)
	{
		isStun = _isStun;
	}
	bool GetIsConfuse() const
	{
		return isConfuse;
	}
	void SetIsConfuse(const bool _isConfuse)
	{
		isConfuse = _isConfuse;
		GetComponent<MovementComponent>()->SetIsConfused(isConfuse);
		if (isConfuse)
		{
			new Timer("ResetConfuse", [this]() {
				isConfuse = false;
				GetComponent<MovementComponent>()->SetIsConfused(false);
				CameraManager::GetInstance().Get("PlayerCamera")->SetRotation(0.0f);
			}, seconds(5.0f), 1, true);
		}
	}
	int GetConfusionEffect()
	{
		return isConfuse ? -1 : 1;
	}
	int* GetChainMultiplier() const
	{
		return chainMultiplier;
	}
	
	PlayerRessource* GetRessources() const
	{
		return ressources;
	}

	void SetCanMove(const bool _canMove)
	{
		GetComponent<MovementComponent>()->SetCanMove(_canMove);
	}

	void ResetLife()
	{
		GetComponent<LifeComponent>()->ResetHealth();
		UpdateLife();
	}

	void SetShouldSave(bool _shouldSave)
	{
		shouldSave = _shouldSave;
	}

	bool GetShouldSave()
	{
		return shouldSave;
	}

public:
	Player(const float _maxHp,const string _id, const Vector2f& _position);
	~Player();

public :
	float LoadHP();
	void SavePlayerStatsData();
	void DeleteSavePlayerStatsData();
	void DeleteSavePurchasedItems();
	void Slide();
	bool ResetChainMultiplier();
	void InitInput();
	void InitLife();
	bool TryToAttack();
	void UpdateLife();
	void UpdateHeartAnimation();
	void UpdateDamageZone();

	void Update() override;
	virtual void DieEvent() override;
	void AddHeart(const int _amount = 1);
	virtual vector<Drawable*> GetDrawables() override
	{
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		_drawables.push_back(visuals);
		for (AttackZone* _zone : damageZone)
		{
			_drawables.push_back(_zone->shape);
		}
		return _drawables;
	}
};