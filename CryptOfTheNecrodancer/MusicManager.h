#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"

#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	float* volume;
	Timer* rythmLoop;
	bool isRunning;
	float* acceptDelay;
	float maxAcceptDelay;
	float minAcceptDelay;
	float playSpeed;
	int currentBPM;
	float tempVolume;

public:
	float* GetAcceptDelay() const
	{
		return acceptDelay;
	}

	void SetAcceptDelay(float _acceptDelay)
	{
		*acceptDelay += _acceptDelay;

		if (*acceptDelay < minAcceptDelay)
		{
			*acceptDelay = minAcceptDelay;
		}
		if (*acceptDelay > maxAcceptDelay)
		{
			*acceptDelay = maxAcceptDelay;
		}
	}

	float* GetVolume() const
	{
		return volume;
	}

	void SetVolume(float _volume)
	{
		*volume = _volume;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(*volume);
		}
	}

public:
	MusicManager();
	~MusicManager();

public:
	void Play(const string& _path, const int _bpm);
	void PlayMusicOnPosition(const string& _path, const int _bpm, const Vector2f& _position);
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