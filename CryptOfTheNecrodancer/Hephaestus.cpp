#include "Hephaestus.h"
#include "SoundManager.h"
#define PATH_HEPHAESTUS "HephaestusSprite.png"

Hephaestus::Hephaestus(const Vector2f& _position) : NPC(PATH_HEPHAESTUS,"Hephaestus", _position)
{
	currentSoundIndex = 0;	

	//AnimationData _animation = AnimationData("ShopKeeper", Vector2f(0, 0), Vector2f(65, 68), READ_RIGHT, ANIM_DIR_NONE, true, 5, 0.1f);
	//components.push_back(new AnimationComponent(this, PATH_HEPHAESTUS, { _animation }, ANIM_DIR_NONE));
	/*zIndex = 1;*/ 
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

	SoundManager::GetInstance().Play(_sounds[currentSoundIndex]);
}
