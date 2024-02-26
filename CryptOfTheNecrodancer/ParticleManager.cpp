#include "ParticleSystem.h"
#include "ParticleManager.h"

void ParticleManager::Update(const float _deltaTime)
{
	for (ParticleSystem* _system : GetAllValues())
	{
		_system->Update(_deltaTime);
	}
	GarbageCollector();
}


void ParticleManager::Draw(RenderWindow& _window)
{
	for (ParticleSystem* _system : GetAllValues())
	{
		_window.draw(*_system);
	}
}
