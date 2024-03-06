#pragma once
#include "Inventory.h"
#include "PlayerRessource.h"
#include "Living.h"
#include "LifeComponent.h"
#include <SFML/Audio.hpp>
#include "MovementComponent.h"
#include "CameraManager.h"
using namespace sf;

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

public:
	bool GetIsStun() const
	{
		return isStun;
	}
	void SetIsStun(const bool _isStun = true)
	{
		isStun = _isStun;
		if (isStun)
		{
			new Timer("ResetStun", [this]() {
				isStun = false;

				}, seconds(0.5f), 1, true);
		}
	}
	bool GetIsConfuse() const
	{
		return isConfuse;
	}
	void SetIsConfuse(const bool _isConfuse)
	{
		isConfuse = _isConfuse;
		if (isConfuse)
		{
			new Timer("ResetConfuse", [this]() {
				isConfuse = false;
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

public:
	Player(const float _maxHp,const float _maxDamages,const string _id, const Vector2f& _position);
	~Player();

public :
	void ResetChainMultiplier();
	void InitInput();
	void InitLife();

	void UpdateLife();
	void UpdateHeartAnimation();

	void Update() override;
	virtual void DieEvent() override;
	virtual vector<Drawable*> GetDrawables() override
	{
		vector<Drawable*> _drawables;
		_drawables.push_back(shape);
		_drawables.push_back(visuals);
		return _drawables;
	}
};