#pragma once

#include "IManager.h"
#include "SoundData.h"
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>, public IManager<string, SoundData>
{
	Sound sound;
	float* volume;
	float tempVolume;

public:
	SoundManager();
	~SoundManager();

public :
	float* GetVolume() const
	{
		return volume;
	}

private:
	void Load(const string& _path);

public:
	void Play(const string& _path);
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleVolume();
};