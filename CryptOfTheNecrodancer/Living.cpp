#include "Living.h"
#include "LifeComponent.h"
#include "DamageComponent.h"
#include "MovementComponent.h"
#include "Pickable.h"
#include "Map.h"

Living::Living(const float _maxHp, const float _maxDammage, const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
	rewardAmount = 0;
	components.push_back(new LifeComponent(this, [&]() { if (type != ET_PLAYER)
	{
		Map::GetInstance().AddOther(new Pickable(rewardAmount, PT_COIN, STRING_ID("Coin"), this->GetPosition()));
		this->Destroy();
	}

		}, false, _maxHp));
	components.push_back(new DamageComponent(this, _maxDammage));
	components.push_back(new MovementComponent(this));
	cooldown = 0;
	currentCooldown = 0;
	indexPatern = 0;
}

Living::Living(const string& _path, const string& _id, const Vector2f& _pos) : Entity(_id, _path, _pos)
{
	rewardAmount = 0;
	cooldown = 0;
	currentCooldown = 0;
	indexPatern = 0;
}
