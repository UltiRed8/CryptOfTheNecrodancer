#pragma once
#include <iostream>

using namespace std;

class Saveable
{
protected:
	string saveLive;

public:
	string GetSaveLive() const
	{
		return saveLive;
	}

public:
	virtual void SetSaveLine() = 0;
};

