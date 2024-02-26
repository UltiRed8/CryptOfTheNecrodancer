#pragma once

#include "MusicData.h"
#include "IManager.h"

#include <SFML/Audio.hpp>

class MusicManager : public IManager<string, MusicData>
{

};