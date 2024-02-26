#pragma once

#include "IManagable.h"

#include <string>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class FontManager;

class FontData : public Font, public IManagable<string>
{

public:
	FontData(const string& _path);

public:
	virtual void Register() override;
};