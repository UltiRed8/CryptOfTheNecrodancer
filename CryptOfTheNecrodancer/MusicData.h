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

	void Play(const string& _path);
	// Hérité via IManagable
	void Register() override;
};