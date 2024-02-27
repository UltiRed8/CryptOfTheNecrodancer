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
	float playSpeed;
	int currentBPM;
	float tempVolume;

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
	void SpeedUp();
	void SpeedDown();
	void SetPlaySpeed(const float _newValue);
	void IncreaseVolume();
	void DecreaseVolume();
	void ToggleVolume();

private:
	void UpdateLoop(const int _bpm);

};