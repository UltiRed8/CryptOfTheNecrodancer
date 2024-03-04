#include "Bat.h"
#include"Macro.h"
#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include"Macro.h"
#include "AnimationComponent.h"

#define PATH_BAT "Entities/BatSprite.png"


Bat::Bat(const Vector2f& _position) :Enemy(100.f,50.f,STRING_ID("Bat"), PATH_BAT, _position, 2)
{
	directionsPatern = {
		Vector2i(1,0),
		Vector2i(-1,0),
		Vector2i(0,1),
		Vector2i(0,-1),
	};

	components.push_back(new AnimationComponent(this, {
		AnimationData("Idle", Vector2f(24, 24), 0, 3, 0.1f, true),
	}, "Idle", shape));

	cooldown = 2;
}

void Bat::SelectDirection()
{
	int _direction = Random((int)directionsPatern.size() - 1);
	GetComponent<MovementComponent>()->SetDirection(directionsPatern[_direction]);
}