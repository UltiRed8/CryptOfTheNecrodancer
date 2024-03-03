#pragma once
#include <SFML/Audio.hpp>
#include "IManagable.h"
#include <iostream>

using namespace std;
using namespace sf;


class MusicData : public Music, public IManagable<string>
{

public:
	MusicData(const string& _id);

private:
	void Register() override;
};