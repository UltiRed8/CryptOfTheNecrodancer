#include "DamageComponent.h"
#include "LifeComponent.h"
#include "Entity.h"
#include "Item.h"
#include "Player.h"

DamageComponent::DamageComponent(Entity* _owner, const float _damagesAmount, const bool _canAttack):Component(_owner)
{
	damagesAmount = _damagesAmount;
	canAttack = _canAttack;
}

bool DamageComponent::Attack(Entity* _target)
{
	float _attack = damagesAmount;

	if (owner->GetType() == ET_PLAYER)
	{
		if (Player* _player = (Player*)owner)
		{
			for (Slot* _slot : _player->GetInventory()->GetSlots())
			{
				if (Item* _item = _slot->GetItem())
				{
					_attack += _item->GetStats().damages * 100.0f;
				}
			}
		}
	}

	if (_attack <= 0.0f) return false;

	LifeComponent* _component = _target->GetComponent<LifeComponent>();
	return _component->ChangeHealth(-_attack);
}

void DamageComponent::Update()
{

}