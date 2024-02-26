#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Object
{
protected:
	vector<Drawable*> drawables;
	int zIndex;

public:
	int GetZIndex() const
	{
		return zIndex;
	}

public:
	Object();

public:
	vector<Drawable*> GetDrawables() const
	{ 
		return drawables; 
	}	
	
	virtual void Update() = 0;
};

