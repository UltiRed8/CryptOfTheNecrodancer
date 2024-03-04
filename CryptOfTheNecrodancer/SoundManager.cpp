#include "SoundManager.h"

SoundManager::SoundManager()
{
	volume = new float(60.0f);
	isMuted = false;
	tempVolume = 0;
}

SoundManager::~SoundManager()
{
	delete volume;
}

void SoundManager::Play(const string& _path, const DirectionalSettings& _settings)
{
	if (_path == "") return;

	SoundData* _soundData = Get(_path);

	if (!_soundData)
	{
		_soundData = new SoundData(_path);
	}

	_soundData->setPosition(Vector3f(_settings.position.x, _settings.position.y, 0.0f));
	_soundData->setRelativeToListener(_settings.attenuationSpeed == 0.0f);
	_soundData->setAttenuation(_settings.attenuationSpeed);
	_soundData->setMinDistance(_settings.minDistance);
	_soundData->setVolume(*volume);
	_soundData->play();
}

void SoundManager::IncreaseVolume()
{
	if (*volume <= 99.0f)
	{
		*volume += 1.0f;
		for (SoundData* _sound : GetAllValues())
		{
			_sound->setVolume(*volume);
		}
	}
}

void SoundManager::DecreaseVolume()
{
	if (*volume >= 1.0f)
	{
		*volume -= 1.0f;
		for (SoundData* _sound : GetAllValues())
		{
			_sound->setVolume(*volume);
		}
	}
}

void SoundManager::ToggleVolume()
{
	isMuted = !isMuted;
	if (isMuted)
	{
		tempVolume = *volume;
		*volume = 0;
		for (SoundData* _sound : GetAllValues())
		{
			_sound->setVolume(0);
		}
	}
	else
	{
		*volume = tempVolume;
		for (SoundData* _sound : GetAllValues())
		{
			_sound->setVolume(*volume);
		}
	}
}