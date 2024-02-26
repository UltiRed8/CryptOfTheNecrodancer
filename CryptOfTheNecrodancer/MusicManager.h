#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	float volume;

public:
	MusicManager();

public:
	void Play(const string& _path);

	void SetVolume(float _volume)
	{
		volume = _volume;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(volume);
		}
	}
};