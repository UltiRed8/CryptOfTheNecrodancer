#include "MusicManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Map.h"
#include "Player.h"
#include "UIImage.h"
#include "MovementComponent.h"
#include "RythmComponent.h"
#include "MenuManager.h"
#include "LightningManager.h"

MusicManager::MusicManager()
{
	volume = new float(10.f);
	rythmLoop = nullptr;
	isRunning = false;
	acceptDelay = new float(300);
	minAcceptDelay = 0.0f;
	maxAcceptDelay = 450.0f;
	playSpeed = 1.0f;
	currentBPM = 0;
	tempVolume = 0.0f;
	delta = 0.0f;
	beatDelay = 0;
	didEvent = false;
}

MusicManager::~MusicManager()
{
	delete volume;
	delete acceptDelay;
}

void MusicManager::Update()
{
	delta += TimerManager::GetInstance().GetDeltaTime();
}

MusicData* MusicManager::GetMusic(const string& _path, const Vector2f& _position)
{
	MusicData* _music = Get(_path);

	if (!_music)
	{
		_music = new MusicData(_path);
		if (!_music->openFromFile("Assets/Music/" + _path + ".ogg"))
		{
			cerr << "La musique n'a pas été correctement chargée ! (" << _path << ".ogg)" << endl;
			return nullptr;
		}
	}

	_music->setVolume(*volume);
	_music->setPosition(_position.x + TILE_SIZE.x / 2.0f, _position.y + TILE_SIZE.y / 2.0f, 0);

	return _music;
}

void MusicManager::Play(const string& _path, const Vector2f& _position, const bool _shouldLoop)
{
	if (_path == "") return;
	Music* _music = GetMusic(_path, _position);
	if (_music)
	{
		_music->setLoop(_shouldLoop);
		_music->play();
	}
}

void MusicManager::PlayMain(const string& _path, const int _bpm, const bool _withShopkeeper, const bool _shouldLoop)
{
	if (_path == "") return;

	StopAll();

	if (isRunning)
	{
		playSpeed = 1.0f;
		currentBPM = _bpm;
		rythmLoop->SetDuration(seconds(1.f / ((currentBPM * playSpeed) / 60.f)));
		rythmLoop->Pause();
	}

	Music* _music = GetMusic(_path, Vector2f(0.0f, 0.0f));
	if (_music)
	{
		_music->setLoop(_shouldLoop);

		MusicData* _current = GetCurrent();
		_current->play();

		if (_withShopkeeper)
		{
			const Vector2f& _shopkeeperPosition = Map::GetInstance().GetShopkeeper()->GetPosition();
			if (MusicData* _music = GetMusic(_current->GetID() + "_shopkeeper", _shopkeeperPosition))
			{
				_music->play();
				_music->setVolume(100.0f);
			}
		}

		UpdateLoop(_bpm);
		isRunning = true;
	}
}

void MusicManager::Toggle()
{
	isRunning = !isRunning;
	isRunning ? Unpause() : Pause();
}

void MusicManager::Pause()
{
	for (MusicData* _music : GetAllValues())
	{
		_music->pause();
	}
	if (rythmLoop)
	{
		rythmLoop->Pause();
	}
}

void MusicManager::StopAll()
{
	for (MusicData* _music : GetAllValues())
	{
		_music->stop();
	}
}

void MusicManager::Unpause() //TODO Fix
{
	for (MusicData* _music : GetAllValues())
	{
		_music->play();
	}
	if (rythmLoop)
	{
		rythmLoop->Resume();
	}
}

void MusicManager::SpeedUp()
{
	if (playSpeed != 1.0f) return;
	cout << "SpeedUp!" << endl;
	SetPlaySpeed(1.125f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	cout << "SpeedDown!" << endl;
	SetPlaySpeed(0.875f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SetPlaySpeed(const float _newValue)
{
	playSpeed = _newValue;
	GetCurrent()->setPitch(playSpeed);
	rythmLoop->SetDuration(seconds(1.f / ((currentBPM* playSpeed) / 60.f)));
}

void MusicManager::IncreaseVolume()
{
	if (*volume >= 0.f && *volume < 100.f)
	{
		GetCurrent()->setVolume(*volume += 1.f);
	}
	else if (*volume >= 100)
	{
		GetCurrent()->setVolume(100);
	}
}

void MusicManager::DecreaseVolume()
{
	if (*volume > 0.f && *volume <= 100.f)
	{
		GetCurrent()->setVolume(*volume -= 1.f);
	}
	else if (*volume <= 0)
	{
		GetCurrent()->setVolume(0);
	}
}

void MusicManager::ToggleVolume()
{
	if (GetCurrent()->getVolume() > 0)
	{
		GetCurrent()->setVolume(0);
		tempVolume = *volume;
		*volume = 0;
	}
	else
	{
		*volume = tempVolume;
		GetCurrent()->setVolume(*volume);
	}
}

void MusicManager::UpdateLoop(const int _bpm)
{
	if (isRunning)
	{
		rythmLoop->Run();
		return;
	}

	if (rythmLoop)
	{
		rythmLoop->Destroy();
	}

	currentBPM = _bpm;

	beatDelay = seconds(1.f / (_bpm / 60.f)).asMilliseconds();
	rythmLoop = new Timer("Timer", [this]() {
		/*return;*/
		new Timer("ResetEvent", [this]() {
			TriggerEvent();
			didEvent = false;
		}, milliseconds(*acceptDelay / 2), 1, true);
		delta = 0;
	}, seconds(1.f / (_bpm / 60.f)), -1);
}

bool MusicManager::TriggerEvent()
{
	if (didEvent) return false;

	const float _delay = *acceptDelay / 2;

	if (delta - 10 <= _delay || delta >= (beatDelay - _delay))
	{
		didEvent = true;
		Map::GetInstance().Update();
		EntityManager::GetInstance().Update();
		return true;
	}
	else
	{
		cout << "bad timing!" << endl;
		return false;
	}
}