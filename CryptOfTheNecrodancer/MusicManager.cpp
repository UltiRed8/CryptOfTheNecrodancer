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
#include "Heart.h"

MusicManager::MusicManager()
{
	rythmType = RT_FREEMOVE;

	currentMain = nullptr;
	currentShopkeeper = nullptr;

	musicPackName = new int(6);
	volume = new float(10.f);
	rythmLoop = nullptr;
	isRunning = false;
	acceptDelay = new float(10);
	minAcceptDelay = 0.0f;
	maxAcceptDelay = 450.0f;
	playSpeed = 1.0f;
	currentBPM = 0;
	tempVolume = 0.0f;
	delta = 0.0f;
	beatDelay = 0;
	didEvent = false;
	needsAnimationUpdate = false;
}

MusicManager::~MusicManager()
{
	delete volume;
	delete acceptDelay;
}

void MusicManager::Update()
{
	delta += TimerManager::GetInstance().GetDeltaTime();
	GarbageCollector();
}

MusicData* MusicManager::GetMusic(const string& _path, const Vector2f& _position)
{
	MusicData* _music = Get(_path);

	if (!_music)
	{
		_music = new MusicData(_path);
		if (!_music->openFromFile("Assets/Music/" + to_string(*musicPackName) + "/" + _path + ".ogg"))
		{
			cerr << "La musique n'a pas été correctement chargée ! (" << _path << ".ogg)" << endl;
			return nullptr;
		}
	}

	_music->setVolume(*volume);
	_music->setPosition(Vector3f(_position.x + TILE_SIZE.x / 2.0f, _position.y + TILE_SIZE.y / 2.0f, 0.0f));

	return _music;
}

void MusicManager::PrepareMain(const string& _path, const int _bpm, const bool _withShopkeeper, const bool _shouldLoop)
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

		if (_withShopkeeper)
		{
			const Vector2f& _shopkeeperPosition = Map::GetInstance().GetShopkeeper()->GetPosition();
			if (currentShopkeeper = GetMusic(currentMain->GetID() + "_shopkeeper", _shopkeeperPosition))
			{
				currentShopkeeper->setLoop(_shouldLoop);
				currentShopkeeper->setVolume(100.0f);
				currentShopkeeper->setMinDistance(75.0f);
				currentShopkeeper->setAttenuation(3.0f);
			}
		}
		else
		{
			currentShopkeeper = nullptr;
		}
	}
}

void MusicManager::UpdateEntitiesAnimations()
{
	for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
	{
		if (AnimationComponent* _animationComponent = _entity->GetComponent<AnimationComponent>())
		{
			for (Animation* _animation : _animationComponent->GetAllValues())
			{
				_animation->SetDuration(1.f / (currentBPM * playSpeed / 60.f));
			}
		}
	}
}

void MusicManager::Play()
{
	UpdateEntitiesAnimations();

	if (currentMain)
	{
		currentMain->play();
	}
	if (currentShopkeeper)
	{
		currentShopkeeper->play();
	}
	UpdateLoop();
	isRunning = true;
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
	SetPlaySpeed(1.125f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(5.0f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	SetPlaySpeed(0.875f);
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
	UpdateEntitiesAnimations();
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
		if (rythmType == RT_NONE) return;
		new Timer("ResetEvent", [this]() {
			TriggerEvent();
			didEvent = false;
		}, milliseconds((Int32)(*acceptDelay / 2)), 1, true);
		delta = 0;
	}, seconds(1.f / (currentBPM / 60.f)), -1);
}

bool MusicManager::TriggerEvent()
{
	if (didEvent && (rythmType != RT_FREEMOVE && rythmType != RT_NONE)) return false;

	const float _delay = *acceptDelay / 2;

	if ((delta - 10 <= _delay || delta >= (beatDelay - _delay)) || rythmType <= RT_FREEMOVE)
	{
		didEvent = true;
		Map::GetInstance().Update();
		EntityManager::GetInstance().Update();
		LightningManager::GetInstance().Update();
		dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->UpdateHeartAnimation();
		return true;
	}
	else
	{
		cout << "bad timing!" << endl;
		return false;
	}
}