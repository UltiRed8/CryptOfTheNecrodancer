#include "Calibration.h"
#include "Macro.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "TimerManager.h"
#include "MusicManager.h"
#include "MenuManager.h"

#define RYTHMHEART0 "UI/RythmHearts0.png"
#define RYTHMHEART1 "UI/RythmHearts1.png"

Calibration::Calibration(const function<void()> _endCallback)
{
	timer = nullptr;
	tries = 20;
	calibrationValue = vector<float>();

	currentTime = 0;
	currentTry = 0;

	new ActionMap("Calibration", {
		{ ActionData("Click",[&]() {RegisterValue(); },{Event::KeyPressed, Keyboard::Space})}
		});
	latency = 0;
	isFinished = false;
	endCallback = _endCallback;
}

Calibration::~Calibration()
{
	timer->Destroy();
}

void Calibration::RegisterValue()
{
	if (isFinished)
	{
		return;
	}
	calibrationValue.push_back(currentTime);
	currentTime = 0;
}

void Calibration::ComputeMSLatency()
{
	float _sum = 0;
	for (const float _value : calibrationValue)
	{
		_sum += _value;
	}
	const int _size = (int)(calibrationValue.size());
	latency = _sum / _size - 500;
	if (latency < -12.0f || latency > 12.0f)
	{
		latency = latency < 0.0f ? -12.0f : 12.0f;
	}
	for (const float _value : calibrationValue)
	{
		EraseElement(calibrationValue, _value);

	}
	calibrationValue.clear();
}

void Calibration::Start()
{
	currentTime = 0;
	isFinished = false;
	timer = new Timer(STRING_ID("Calibration"), [&]() {
		Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("Calibration")->Get("RythmHearts"))->GetShape();
		TextureManager::GetInstance().Load(_shape, RYTHMHEART1);
		new Timer("HeartIndicatorReset", [this]() {
			Shape* _shape = dynamic_cast<UIImage*>(MenuManager::GetInstance().Get("Calibration")->Get("RythmHearts"))->GetShape();
			TextureManager::GetInstance().Load(_shape, RYTHMHEART0);
			}, seconds(0.1f), 1, true);
		SoundManager::GetInstance().Play("Assets/Sounds/en_general_hit.ogg");
		currentTry++;
	}, milliseconds(500),tries, false);
}

void Calibration::Update()
{
	if (isFinished)
	{
		return;
	}
	currentTime += (int)TimerManager::GetInstance().GetDeltaTime();
	if (currentTry >= tries)
	{
		currentTime = 0;
		currentTry = 0;
		isFinished = true;
		timer->Pause();
		ComputeMSLatency();
		MusicManager::GetInstance().SetLatency(latency);
		if (endCallback)
		{
			endCallback();
		}
	}
}
