#include "Hephaestus.h"
#include "SoundManager.h"
#include "Macro.h"
#define PATH_HEPHAESTUS "HephaestusSprite.png"

Hephaestus::Hephaestus(const Vector2f& _position) : Entity("Hephaestus", PATH_HEPHAESTUS, _position)
{
	currentSoundIndex = 0;	

	components.push_back(new AnimationComponent(this, PATH_HEPHAESTUS, {
		AnimationData("AnvilHit", Vector2f(65, 68), 0, 4, (1.f / (130 / 60.f)) / 4.0f, false),
	}, "AnvilHit", shape));

	shape->setScale(3.0f, 3.0f);
	shape->move(Vector2f(-1.0f, -1.0f) * TILE_SIZE);

	zIndex = 2; 
	type = ET_EPHAESTUS;
}

void Hephaestus::Update()
{
	currentSoundIndex++;
	if (currentSoundIndex >= 4)
	{
		currentSoundIndex = 0;
	}

	vector<string> _sounds = 
	{   
		"Assets/Sounds/Hammer1.ogg",
		"Assets/Sounds/Hammer2.ogg",
		"Assets/Sounds/Hammer3.ogg",
		"Assets/Sounds/Hammer4.ogg",
	};

	SoundManager::GetInstance().Play(_sounds[currentSoundIndex], DirectionalSettings(GetPosition() + TILE_SIZE / 2.0f, 175, 10.0f));
	Entity::Update();
}