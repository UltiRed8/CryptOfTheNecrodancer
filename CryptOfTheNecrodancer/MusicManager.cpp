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
	acceptDelay = new float(300.0f);
	minAcceptDelay = 0.0f;
	maxAcceptDelay = 450.0f;
	playSpeed = 1.0f;
	currentBPM = 0;
	tempVolume = 0.0f;
}

MusicManager::~MusicManager()
{
	delete volume;
	delete acceptDelay;
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

		new Timer("StartMusicDelay", [&]() {
			MusicData* _current = GetCurrent();
			_current->play();
		}, milliseconds((Int32)(*acceptDelay / 2)), 1, true);

		if (_withShopkeeper)
		{
			new Timer("StartMusicDelayShopkeeper", [&]() {
				MusicData* _current = GetCurrent();
				_current->play();
				Vector2f _shopkeeperPosition = EntityManager::GetInstance().Get("Player")->GetPosition();// = Map::GetInstance().GetShopkeeper().GetPosition(); // TODO
				if (MusicData* _music = GetMusic(_current->GetID() + "_shopkeeper", _shopkeeperPosition))
				{
					_music->play();
					_music->setVolume(100.0f);
				}
			}, milliseconds((Int32)(*acceptDelay / 2)), 1, true);
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

	rythmLoop = new Timer("Timer", [this]() {

		for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
		{
			if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
			{
				_component->BeforeUpdate();
			}
		};

		new Timer("InputsTooSoon", [this]() {
			Map::GetInstance().Update();
			
			for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
			{
				if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
				{
					_component->TimedUpdate();
				}
			};

			Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
			TextureManager::GetInstance().Load(_shape, "RythmHearts1.png");
			new Timer("HeartIndicatorReset", [this]() {
				Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
				TextureManager::GetInstance().Load(_shape, "RythmHearts0.png");
				}, seconds(0.1f), 1, true);
			
		}, milliseconds((Int32)(*acceptDelay / 2)), 1, true);

		new Timer("InputsTooLate", [this]() {
			for (Entity* _entity : EntityManager::GetInstance().GetAllValues())
			{
				if (RythmComponent* _component = _entity->GetComponent<RythmComponent>())
				{
					_component->AfterUpdate();
				}
			};
		}, milliseconds((Int32)(*acceptDelay)), 1, true);

	}, seconds(1.f / (_bpm / 60.f)), -1);
}