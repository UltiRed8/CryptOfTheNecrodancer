#include "NPC.h"
#include "Macro.h"
#include "SoundManager.h"

#define PATH_SHOPKEEPER "Entities/ShopkeeperSprite.png"
#define PATH_MERLIN "Entities/MerlinSprite.png"
#define PATH_HEPHAESTUS "Entities/HephaestusSprite.png"

NPC::NPC(const NPCType& _type, const Vector2f& _position) : Living(900, 1000, "", STRING_ID("NPC"), _position)
{
	zIndex = 1;
	currentCooldown = 0;
	type = _type;
	ChangeTexture();
	UpdateAnimation();
}

void NPC::ChangeTexture()
{
	string _paths[] = {
		PATH_SHOPKEEPER,
		PATH_MERLIN,
		PATH_HEPHAESTUS,
	};

	TextureManager::GetInstance().Load(shape, _paths[type]);
}

void NPC::UpdateAnimation()
{
	if (type == NPC_MERLIN)
	{
		components.push_back(new AnimationComponent(this, {
			AnimationData("Left", Vector2f(32, 39), 0, 2, 0.1f, false),
			AnimationData("Right", Vector2f(32, 39), 3, 2, 0.1f, false),
		}, "Left", shape));
		shape->setScale(1.5f, 1.5f);
		shape->move(Vector2f(-TILE_SIZE.x / 2.5f, -TILE_SIZE.y / 2.0f));
	}
	else if (type == NPC_SHOPKEEPER)
	{
		components.push_back(new AnimationComponent(this, {
			AnimationData("ShopKeeper", Vector2f(47, 47), 4, 3, 0.1f, false),
		}, "ShopKeeper", shape));
		shape->setScale(2.0f, 2.0f);
		shape->move(Vector2f(-0.5f, -1.5f) * TILE_SIZE);
	}
	else
	{
		components.push_back(new AnimationComponent(this, {
			AnimationData("AnvilHit", Vector2f(65, 68), 0, 4, 0.1f, false),
		}, "AnvilHit", shape));
		shape->setScale(3.0f, 3.0f);
		shape->move(Vector2f(-1.0f, -1.0f) * TILE_SIZE);
	}
}

void NPC::DieEvent()
{
}

void NPC::Update()
{

	if (type == NPC_MERLIN)
	{
		currentCooldown++;
		if (currentCooldown >= 2)
		{
			currentCooldown = 0;
		}
		if (AnimationComponent* _animationComponent = GetComponent<AnimationComponent>())
		{
			_animationComponent->SetPlayingID(currentCooldown == 1 ? "Left" : "Right");
		}
		for (Component* _component : components)
		{
			_component->Update();
		}
		return;
	}
	else if (type == NPC_HEPHAESTUS)
	{
		currentCooldown++;
		if (currentCooldown > 3)
		{
			currentCooldown = 0;
		}

		vector<string> _sounds = {
			"Assets/Sounds/Hammer1.ogg",
			"Assets/Sounds/Hammer2.ogg",
			"Assets/Sounds/Hammer3.ogg",
			"Assets/Sounds/Hammer4.ogg",
		};

		SoundManager::GetInstance().Play(_sounds[currentCooldown], DirectionalSettings(GetPosition() + TILE_SIZE / 2.0f, 175, 10.0f));
	}
	Entity::Update();
}