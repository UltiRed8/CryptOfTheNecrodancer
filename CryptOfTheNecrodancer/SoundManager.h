#pragma once

#include "IManager.h"
#include "SoundData.h"
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>, public IManager<string, SoundData>
{
	Sound sound;

public:
	SoundManager();

private:
	void Load(const string& _path);

public:
	void Play(const string& _path);
};