#pragma once
#include "Living.h"
#include "MovementComponent.h"
#include "CollisionComponent.h"	
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "Pickable.h"
#include "LightSource.h"
#include "MenuManager.h"

class Enemy : public Living
{
protected:
	RectangleShape* visuals;
	int rewardAmount;
	int indexPatern;
	int currentCooldown;
	int cooldown;
	vector<Vector2i>directionsPatern;

public:
	int GetRewardAmount() const
	{
		return rewardAmount;
	}

public:
	Enemy(const float _maxHp,const float _maxDammage,const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins);

public:
	void DieEvent();
	void InitLifeUI();
	virtual void SelectDirection() = 0;
	virtual void Update() override;
	virtual vector<Drawable*> GetDrawables() override
	{
		vector<Drawable*> _drawable = GetComponent<LifeComponent>()->GetHealthBar()->GetDrawables();
		vector<Drawable*> _drawables;
		_drawables.insert(_drawables.end(), _drawable.begin(), _drawable.end());
		_drawables.push_back(shape);
		_drawables.push_back(visuals);
		return _drawables;
	}
};