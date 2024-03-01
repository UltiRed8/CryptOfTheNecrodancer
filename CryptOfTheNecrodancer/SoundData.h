#pragma once

#include "IManagable.h"

#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

class SoundManager;

class SoundData : public Sound, public IManagable<string>
{
	SoundBuffer buffer;

public:
	SoundData(const string& _path);

public:
	virtual void Register() override;
};