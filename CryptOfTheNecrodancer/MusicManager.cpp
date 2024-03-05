
#include "MusicManager.h"
#include "EntityManager.h"
#include "TimerManager.h"
#include "Map.h"
#include "Player.h"
#include "UIImage.h"
#include "MovementComponent.h"
#include "MenuManager.h"
#include "LightningManager.h"
#include "Heart.h"
#include "RythmIndicator.h"
#include "SoundManager.h"

#define PATH_RYTHM_INDICATOR_BLUE "UI/BeatMarkerBlue.png"
#define PATH_RYTHM_INDICATOR_RED "UI/BeatMarkerRed.png"
#define PATH_HEART1 "UI/RythmHearts0.png"
#define PATH_HEART2 "UI/RythmHearts1.png"
#define PATH_STAIR "Dungeons/Stairs.png"

#define SOUND_RYTHM_FAILED "Assets/Sounds/sfx_missedbeat.ogg"

MusicManager::MusicManager()
{
	rythmType = RT_FREEMOVE;

	currentMain = nullptr;
	currentShopkeeper = nullptr;
	latency = 0.0f;
	musicPackName = new int(1);
	volume = new float(10.f);
	rythmLoop = nullptr;
	isRunning = false;
	acceptDelay = new float(50);
	minAcceptDelay = 0.0f;
	maxAcceptDelay = 450.0f;
	playSpeed = 1.0f;
	currentBPM = 0;
	tempVolume = 0.0f;
	delta = 0.0f;
	beatDelay = 0;
	didEvent = false;
	needsAnimationUpdate = false;
	currentTime = 0;
	maxTime = 0;
}

MusicManager::~MusicManager()
{
	delete volume;
	delete musicPackName;
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
		if (!_music->openFromFile("Assets/Music/" + _path + ".ogg"))
		{
			cerr << "La musique n'a pas �t� correctement charg�e ! (" << _path << ".ogg)" << endl;
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
	if (currentMain = GetMusic(to_string(*musicPackName) + "/" + _path, Vector2f(0.0f, 0.0f)))
	{
		currentMain->setLoop(_shouldLoop);

		if (_withShopkeeper)
		{
			const Vector2f& _shopkeeperPosition = Map::GetInstance().GetShopkeeper()->GetPosition();
			if (currentShopkeeper = GetMusic(currentMain->GetID() + "_shopkeeper", _shopkeeperPosition))
			{
				currentShopkeeper->setLoop(_shouldLoop);
				currentShopkeeper->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
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
	for (UIElement* _element : MenuManager::GetInstance().Get("HUD")->GetAllValues())
	{
		if (RythmIndicator* _indicator = dynamic_cast<RythmIndicator*>(_element))
		{
			_indicator->Destroy();
		}
	}

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
	}, seconds(6.5f), 1, true);
}

void MusicManager::SpeedDown()
{
	if (playSpeed != 1.0f) return;
	SetPlaySpeed(0.875f);
	new Timer("ResetPlaySpeed", [this]() {
		SetPlaySpeed(1.0f);
	}, seconds(6.5f), 1, true);
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
	if (*volume <= 99.0f)
	{
		*volume += 1.0f;
		for (MusicData* _music : GetAllValues())
		{
				_music->setVolume(*volume);
		}
		if (currentShopkeeper)
		{
			currentShopkeeper->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
		}
	}
}

void MusicManager::DecreaseVolume()
{
	if (*volume >= 1.0f)
	{
		*volume -= 1.0f;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(*volume);
		}
		if (currentShopkeeper)
		{
			currentShopkeeper->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
		}
	}
}

void MusicManager::ToggleVolume()
{
	if (GetCurrent()->getVolume() > 0)
	{
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(0);
		}
		tempVolume = *volume;
		*volume = 0;
	}
	else
	{
		*volume = tempVolume;
		for (MusicData* _music : GetAllValues())
		{
			_music->setVolume(*volume);
		}
		if (currentShopkeeper)
		{
			currentShopkeeper->setVolume(*volume == 0.0f ? 0.0f : *volume + 50.0f);
		}
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

	Time _duration = currentMain->getDuration();
	const int _max = _duration.asMilliseconds() * 80 / 100;
	Time _curentDuration = currentMain->getPlayingOffset();

	string _path ;
	if (currentMain->getStatus() == SoundSource::Stopped)
	{
		//Player* _player = dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player")); // TODO
		//_player->SetCanMove(false);
		//Entity* _stair = new Entity(STRING_ID("Stair"), PATH_STAIR, _player->GetPosition());
		//_stair->SetZIndex(1);
		//new Timer("StairTimer", [&]() {Map::GetInstance().OpenPrepared(); /*_stair->Destroy()*/; }, seconds(2.0f), 1, true);
		//_player->SetCanMove(true);
		if (MenuManager::GetInstance().BlockPlayer()) return false;
		Map::GetInstance().OpenPrepared();
	}
	if (currentMain->getLoop())
	{
		_path = PATH_RYTHM_INDICATOR_BLUE;
	}
	else
	{
		_path = _curentDuration.asMilliseconds() > _max ? PATH_RYTHM_INDICATOR_RED : PATH_RYTHM_INDICATOR_BLUE;
	}

	if ((delta <= _delay + latency || delta >= (/*beatDelay -*/ _delay - latency)) || rythmType <= RT_FREEMOVE)
	{
   		Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
		TextureManager::GetInstance().Load(_shape, PATH_HEART2);
		new Timer("HeartIndicatorReset", [this]() {
			Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("HUD")->Get("RythmHearts"))->GetShape();
			TextureManager::GetInstance().Load(_shape, PATH_HEART1);
		}, seconds(0.1f), 1, true);
		didEvent = true;
		Menu* _hud = MenuManager::GetInstance().Get("HUD");
		if (currentMain->getStatus() == SoundSource::Playing)
		{
			new RythmIndicator(RID_RIGHT,_hud, _path);
			new RythmIndicator(RID_LEFT,_hud, _path);
		}
		
		Map::GetInstance().Update();
		EntityManager::GetInstance().Update();
		LightningManager::GetInstance().Update();
		dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->UpdateHeartAnimation();
		return true;
	}
	else
	{
		SoundManager::GetInstance().Play(SOUND_RYTHM_FAILED);
		dynamic_cast<Player*>(EntityManager::GetInstance().Get("Player"))->ResetChainMultiplier();
		return false;
	}
}