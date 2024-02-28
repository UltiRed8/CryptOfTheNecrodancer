#include "CollisionComponent.h"
#include"EntityManager.h"
CollisionComponent::CollisionComponent(Entity* _owner) : Component(_owner)
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

		if (_shape)
		{
			if (_shape == _currentShape) continue;

			if (_rect.intersects(_shape->getGlobalBounds()))
			{
				const EntityType& _entityType = _entity->GetType();

				for (const CollisionReaction& _reaction : _reactions)
				{
					if (_entityType == _reaction.type)
					{
						_reaction.callback(_entity);
					}
				}
			}
		}
	}
}
