#include "Slime.h"
#include "Macro.h"

#include"DamageComponent.h"
#include"LifeComponent.h"
#include "RythmComponent.h"
#include "AnimationComponent.h"


Slime::Slime(const Vector2f& _position, const string& _path) : Enemy(STRING_ID("Slime"), _path, _position)
{
    slimeType = S_NONE;
    AnimationData _animation = AnimationData(STRING_ID("Slimitus Nutritus"), Vector2f(0, 0), Vector2f(26, 26), READ_RIGHT, ANIM_DIR_NONE, true, 4, 0.1f);
    components.push_back(new DamageComponent(this, 0.5));
    components.push_back(new LifeComponent(this, [this]() {DieEvent(); }, false, 1));
    components.push_back(new RythmComponent(this,nullptr, [this]() { UpdateRythm(); },nullptr ));
    components.push_back(new AnimationComponent(this, _path, { _animation }, ANIM_DIR_NONE));
}

void Slime::DieEvent()
{
}

void Slime::SetNextDirection()
{
    GetComponent< MovementComponent>()->SetDirection(directionsPatern[indexPatern], false);
    indexPatern++;
    if (indexPatern >= directionsPatern.size())
    {
        indexPatern = 0;
    }
}

void Slime::UpdateRythm()
{
    currentCooldown--;
    if (currentCooldown <= 0)
    {
        currentCooldown = cooldown;
        if (GetComponent< MovementComponent>()->GetCanMove())
        {
            SetNextDirection();
        }
    }
}
