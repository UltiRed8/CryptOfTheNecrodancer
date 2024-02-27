#include "CollisionComponent.h"
#include"EntityManager.h"
#include"Entity.h"
CollisionComponent::CollisionComponent(Entity* _owner):Component(_owner)
{
}

void CollisionComponent::Update()
{
}

void CollisionComponent::CheckCollision(const vector<CollisionReaction>& _reactions)
{
	Shape* _currentShape = owner->GetShape();
	FloatRect _rect = _currentShape->getGlobalBounds();
	for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
	{
		Shape* _shape = _entity->GetShape();

		if (_shape == _currentShape) continue;

		if (_rect.intersects(_shape->getGlobalBounds()))
		{
			

			for ( CollisionReaction _reaction : _reactions)
			{
				
					_reaction.callback(_entity);
				
			}
		}
	}
}
