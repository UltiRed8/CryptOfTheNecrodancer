#include "Bomb.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "Macro.h"
#include "Map.h"
#include "Trap.h"

#define PATH_BOMB "Entities/Explosion.png"

#define SOUND_BOMB_LIT "Assets/Sounds/sfx_bomb_lit.ogg"
#define SOUND_EXPLOSION "Assets/Sounds/sfx_bomb_explode.ogg"

Bomb::Bomb(const Vector2f& _position) : Placeable(STRING_ID("Bomb"), "", _position)
{
	cooldown = 4;
	zIndex = 1;
	TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_BOMB, 0, Vector2i(74, 74));
	SoundManager::GetInstance().Play(SOUND_BOMB_LIT);
}

void Bomb::Explode()
{
	shape->setScale(3.0f, 3.0f);

	shape->move(Vector2f(-1.0f, -1.0f) * TILE_SIZE);

	new Timer(STRING_ID("ExplosionTimer"), [this]() {
		cooldown--;
		TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_BOMB, 4 - cooldown, Vector2i(74, 74));
	}, seconds(0.1f), 7, true);
	new Timer(STRING_ID("ExplosionTimerEnd"), [this]() {
		Destroy();
	}, seconds(0.7f), 1, true);

	SoundManager::GetInstance().Play(SOUND_EXPLOSION);

	vector<Entity*> _allEntities = EntityManager::GetInstance().GetAllValues();

	for (Entity* _entity : _allEntities)
	{
		if (_entity->GetShape()->getGlobalBounds().intersects(shape->getGlobalBounds()))
		{
			if (Wall* _wall = dynamic_cast<Wall*>(_entity))
			{
				_wall->DestroyWall(true);
			}
			else if (Enemy* _enemy = dynamic_cast<Enemy*>(_entity))
			{
				_enemy->GetComponent<LifeComponent>()->ChangeHealth(-400.0f);
			}
			else if (Player* _player = dynamic_cast<Player*>(_entity))
			{
				_player->GetComponent<LifeComponent>()->ChangeHealth(-200.0f);
			}
			else if (Trap* _trap = dynamic_cast<Trap*>(_entity))
			{
				_trap->Destroy();
			}
		}
	}
}

void Bomb::Update()
{
	if (cooldown >= 0)
	{
		cooldown--;
		TextureManager::GetInstance().LoadFromTextureSheet(shape, PATH_BOMB, 4 - cooldown, Vector2i(74, 74));
		if (cooldown == 0)
		{
			Explode();
		}
	}
}