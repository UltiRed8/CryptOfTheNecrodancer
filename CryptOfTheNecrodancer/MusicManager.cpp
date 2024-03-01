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
	debugMode = false;

	currentMain = nullptr;
	currentShopkeeper = nullptr;

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

void MusicManager::PlayMain(const string& _path, const int _bpm, const bool _withShopkeeper, const bool _shouldLoop)
{
	if (_path == "") return;

	StopAll();

	playSpeed = 1.0f;
	currentBPM = _bpm;
	if (rythmLoop)
	{
		rythmLoop->SetDuration(seconds(1.f / ((currentBPM * playSpeed) / 60.f)));
		rythmLoop->Pause();
	}

	if (currentMain = GetMusic(_path, Vector2f(0.0f, 0.0f)))
	{
		currentMain->setLoop(_shouldLoop);
		currentMain->play();

		if (_withShopkeeper)
		{
			const Vector2f& _shopkeeperPosition = Map::GetInstance().GetShopkeeper()->GetPosition();
			if (currentShopkeeper = GetMusic(currentMain->GetID() + "_shopkeeper", _shopkeeperPosition))
			{
				currentShopkeeper->setLoop(_shouldLoop);
				currentShopkeeper->play();
				currentShopkeeper->setVolume(100.0f);
			}
		}
		else
		{
			currentShopkeeper = nullptr;
		}

		UpdateLoop();
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
	if (currentMain)
	{
		currentMain->pause();
	}
	if (currentShopkeeper)
	{
		currentShopkeeper->pause();
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

void MusicManager::Unpause()
{
	if (currentMain)
	{
		currentMain->play();
	}
	if (currentShopkeeper)
	{
		currentShopkeeper->play();
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
	SetPlaySpeed(1.5f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	cout << "SpeedDown!" << endl;
	SetPlaySpeed(0.5f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SetPlaySpeed(const float _newValue)
{
	playSpeed = _newValue;

	if (currentMain)
	{
		currentMain->setPitch(playSpeed);
	}
	if (currentShopkeeper)
	{
		currentShopkeeper->setPitch(playSpeed);
	}
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

void MusicManager::UpdateLoop()
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

	beatDelay = seconds(1.f / (currentBPM / 60.f)).asMilliseconds();
	rythmLoop = new Timer("Timer", [this]() {
		if (debugMode) return;
		new Timer("ResetEvent", [this]() {
			TriggerEvent();
			didEvent = false;
		}, milliseconds(*acceptDelay / 2), 1, true);
		delta = 0;
	}, seconds(1.f / (currentBPM / 60.f)), -1);
}

bool MusicManager::TriggerEvent()
{
	if (!debugMode)
	{
		if (didEvent) return false;
	}

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