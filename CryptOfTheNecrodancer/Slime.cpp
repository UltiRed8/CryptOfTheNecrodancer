#include "Slime.h"
#include "Macro.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"


Slime::Slime(const Vector2f& _position, const string _path) : Enemy(STRING_ID("Slime"), _path, _position)
{
	// Random Slime Type
	slimeType = static_cast<SlimeType>(Random(2, 0));

	vector<Vector2i> _greenDirectionsPatern = {
		Vector2i(0,0),
	};
	vector<Vector2i> _blueDirectionsPatern = {
		Vector2i(-1,0),
		Vector2i(1,0),
	};
	vector<Vector2i> _orangeDirectionsPatern = {
		Vector2i(1,0),
		Vector2i(-1,0),
		Vector2i(0,1),
		Vector2i(0,-1),
	};
	
	vector< vector<Vector2i> > _directionsPatern = {
		_blueDirectionsPatern,
		_greenDirectionsPatern,
		_orangeDirectionsPatern,
	};

	directionsPatern = _directionsPatern[slimeType];


	AnimationData _animation = AnimationData(STRING_ID("Slimitus Nutritus"), Vector2f(0, 0), Vector2f(26, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
	components.push_back(new MovementComponent(this));
	components.push_back(new DamageComponent(this, 0.5));
	components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, 1));
	components.push_back(new RythmComponent(this, { [this]() { UpdateRythm(); } }));
	components.push_back(new AnimationComponent(this, _path, {_animation}, ANIM_DIR_NONE));
}

void Slime::DieEvent()
{
}

void Slime::SelectDirection()
{
	int _direction = Random((int)directionsPatern.size());
	GetComponent< MovementComponent>()->SetDirection(directionsPatern[_direction], false);
}

void Slime::SelectDirection(const int _direction)
{
	GetComponent< MovementComponent>()->SetDirection(directionsPatern[_direction], false);
}

void Slime::SetNextDirection()
{
	GetComponent< MovementComponent>()->SetDirection(directionsPatern[indexPattern], false);
	indexPattern++;
	if (indexPattern >= directionsPatern.size())
	{
		indexPattern = 0;
	}
}

void Slime::UpdateRythm()
{
	if (GetComponent< MovementComponent>()->GetCanMove())
	{
		SetNextDirection();
	}
}
