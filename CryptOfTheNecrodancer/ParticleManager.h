#pragma once

#include "Singleton.h"
#include "IManager.h"

class ParticleSystem;

class ParticleManager : public Singleton<ParticleManager>, public IManager<string, ParticleSystem>
{

public:
	void Update(const float _deltaTime);
	void Draw(RenderWindow& _window);
};