#include "Bat.h"
#include"Macro.h"
#include"MovementComponent.h"
#include"DamageComponent.h"
#include"LifeComponent.h"
#include"Macro.h"
#define PATH_BAT ""
Bat::Bat(const Vector2f& _position) :Enemy(STRING_ID("Bat"), PATH_BAT, _position)
{
	directionsPatern = {
		Vector2i(1,0),
		Vector2i(-1,0),
		Vector2i(0,1),
		Vector2i(0,-1),
	};
	components.push_back(new MovementComponent(this));
		components.push_back(new DamageComponent(this, 0.5));
		components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, 1));

}

void Bat::DieEvent()
{
	//Drop 2 gold
}

void Bat::SelectDirection()
{
	int _direction=Random((int)directionsPatern.size());
	GetComponent< MovementComponent>()->SetDirection(directionsPatern[_direction]);

}

void Bat::UpdateRythm()
{
	if (GetComponent< MovementComponent>()->GetCanMove())
	{

	}
}
