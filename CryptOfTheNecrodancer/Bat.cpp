#include "Bat.h"
#include"Macro.h"
#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include"Macro.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"

#define PATH_BAT "BatSprite.png"


Bat::Bat(const Vector2f& _position) :Enemy(STRING_ID("Bat"), PATH_BAT, _position, 2)
{
	directionsPatern = {
		Vector2i(1,0),
		Vector2i(-1,0),
		Vector2i(0,1),
		Vector2i(0,-1),
	};

	//AnimationData _animation = AnimationData(STRING_ID("Bat"), Vector2f(0, 0), Vector2f(24, 24), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new RythmComponent(this,nullptr, [this]() { UpdateRythm(); }	,nullptr ));
	//components.push_back(new AnimationComponent(this, PATH_BAT, { _animation }, ANIM_DIR_NONE));
	cooldown = 2;
	InitAllBattlesComponents(50.f, 100.f);
}

void Bat::SelectDirection()
{
	int _direction=Random((int)directionsPatern.size() - 1);
	GetComponent< MovementComponent>()->SetDirection(directionsPatern[_direction],false);

}

void Bat::UpdateRythm()
{
	currentCooldown--;
	if (currentCooldown <= 0)
	{
		currentCooldown = cooldown;
		if (GetComponent< MovementComponent>()->GetCanMove())
		{
			SelectDirection();
		}
	}
}
