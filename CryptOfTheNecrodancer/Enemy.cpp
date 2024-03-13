#include "Enemy.h"
#include "Map.h"
#include "Heart.h"
#include "SoundManager.h"

#define PATH_SHADOW "Entities/Shadow.png"

Enemy::Enemy(const float _maxHp, const float _maxDammage, const string& _id, const string& _path, const Vector2f& _position, const int _droppedCoins)
	: Living(_maxHp,_maxDammage, PATH_SHADOW, _id,_position)
{
	indexPatern = 0;
	zIndex = 3;
	currentCooldown = 0;
	cooldown = 0;

	visuals = new RectangleShape(TILE_SIZE);
	TextureManager::GetInstance().Load(visuals, _path);
	visuals->setPosition(_position + Vector2f(0.0f, -0.5f) * TILE_SIZE);

	MovementComponent* _movement = GetComponent<MovementComponent>();
	CollisionComponent* _collision = new CollisionComponent(this);
	components.push_back(_collision);
	_movement->InitCollisions(_collision, {
		CollisionReaction(ET_WALL, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			return true;
		}),
		CollisionReaction(ET_DOOR, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			return true;
		}),
		CollisionReaction(ET_ENEMY, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			indexPatern--;
			return true;
		}),
		CollisionReaction(ET_PLAYER, [this](Entity* _entity) {
			GetComponent<MovementComponent>()->UndoMove();
			GetComponent<DamageComponent>()->Attack(_entity);
			SoundManager::GetInstance().Play(attackSound);
			indexPatern--;
			dynamic_cast<Player*>(_entity)->ResetChainMultiplier();
			return true;
		}),
	});
	rewardAmount = _droppedCoins;
	type = ET_ENEMY;
}

void Enemy::DieEvent()
{
	Map::GetInstance().AddOther(new Pickable(PIT_COIN, STRING_ID("Coin"), this->GetPosition(), rewardAmount));
	SoundManager::GetInstance().Play(deathSound);
	this->Destroy();
}

void Enemy::InitLifeUI()
{
	Menu* _ennmyLife = new Menu("PlayerLife", {}, 7, false);
	_ennmyLife->Open();
}

void Enemy::Update()
{
	currentCooldown--;
	if (currentCooldown <= 0)
	{
		currentCooldown = cooldown;
		SelectDirection();
	}
	Entity::Update();
	visuals->setPosition(shape->getPosition() + Vector2f(0.0f, -0.5f) * TILE_SIZE);
}