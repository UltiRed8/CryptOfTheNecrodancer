#include "DamageComponent.h"
#include"LifeComponent.h"
#include"Entity.h"
DamageComponent::DamageComponent(Entity* _owner, const float _damagesAmount, const bool _canAttack):Component(_owner)
{
	damagesAmount = _damagesAmount;
	canAttack = _canAttack;
}

bool DamageComponent::Attack(Entity* _target)
{
	LifeComponent* _component = _target->GetComponent<LifeComponent>();
	return _component->ChangeHealth(-damagesAmount);
}

void DamageComponent::Update()
{

}