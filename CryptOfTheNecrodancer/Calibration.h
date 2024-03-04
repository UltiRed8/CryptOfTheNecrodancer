#pragma once
#include "Timer.h"
class Calibration
{
	Timer* timer;
	int tries;
	vector<float> calibrationValue;
	float currentTime;
	int currentTry;
	float latency;
	bool isFinished;
	function<void()> endCallback;
	

public:
	float GetLatency()
	{
		return latency; 
	}

public:
	Calibration(const function<void()> _endCallback);

private:
	void RegisterValue();
	void ComputeMSLatency();


public:
	void Start();

	void Update();
};

