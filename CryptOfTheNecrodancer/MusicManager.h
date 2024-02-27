#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"

#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	float volume;
	Timer* rythmLoop;
	bool isRunning;
	int acceptDelay;

public:
	void SetVolume(float _volume)
	{
		volume = _volume;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(volume);
		}
	}

public:
	MusicManager();

public:
	void Play(const string& _path, const int _bpm);
	void Toggle();
	void Pause();
	void Unpause();
	void Temp();

private:
	void UpdateLoop(const int _bpm);

};