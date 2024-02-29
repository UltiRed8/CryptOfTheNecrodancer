#pragma once

#include "MusicData.h"
#include "IManager.h"
#include "Singleton.h"
#include "Timer.h"

#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>, public Singleton<MusicManager>
{
	int acceptDelay;
	int currentBPM;
	bool isRunning;
	float playSpeed;
	float tempVolume;
	float* volume;
	Timer* rythmLoop;

public:
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
	MusicData* GetMusic(const string& _path, const Vector2f& _position);
	void Play(const string& _path, const Vector2f& _position, const bool _shouldLoop = false);
	void PlayMain(const string& _path, const int _bpm, const bool _withShopkeeper = false, const bool _shouldLoop = false);
	void StopAll();
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