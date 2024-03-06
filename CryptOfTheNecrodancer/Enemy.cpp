#include "Enemy.h"
#include "Map.h"
#include "Heart.h"

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
			indexPatern--;
			dynamic_cast<Player*>(_entity)->ResetChainMultiplier();
			return true;
		}),
	});
	rewardAmount = _droppedCoins;
	type = ET_ENEMY;
}
//Tu part de la fin des coeur 

void Enemy::DieEvent()
{
	Map::GetInstance().AddOther(new Pickable(rewardAmount, PT_COIN, STRING_ID("Coin"), this->GetPosition()));
	this->Destroy();
}

void Enemy::InitLifeUI()
{
	//TODO Changer position
	Menu* _ennmyLife = new Menu("PlayerLife", {
		/*new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * 4.2f, SCREEN_HEIGHT - 55 * 12.8)),
		new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * (4.2f + 1.0f), SCREEN_HEIGHT - 55 * 12.8)),
		new Heart(STRING_ID("Hearts"), Vector2f(25.0f, 25.0f) * 2.0f, Vector2f(SCREEN_WIDTH - 55 * (4.2f + 2.0f), SCREEN_HEIGHT - 55 * 12.8)),*/
		}, 7, false);

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