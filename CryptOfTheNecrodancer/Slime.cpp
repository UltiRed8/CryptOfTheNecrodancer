#include "Slime.h"
#include "Macro.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "AnimationComponent.h"


Slime::Slime(const float _maxHp, const float _maxDammage, const Vector2f& _position, const string& _path, const int _droppedCoins) : Enemy(_maxHp, _maxDammage, STRING_ID("Slime"), _path, _position, _droppedCoins)
{
	slimeType = S_NONE;
	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(26, 26), 0, 3, 0.1f, false),
	}, "Idle", shape));
}

void Slime::SetNextDirection()
{
	GetComponent<MovementComponent>()->SetDirection(directionsPatern[indexPatern]);
	indexPatern++;
	if (indexPatern >= directionsPatern.size())
	{
		indexPatern = 0;
	}
}

void Slime::SelectDirection()
{
	if (GetComponent<MovementComponent>()->GetCanMove())
	{
		SetNextDirection();
	}
}