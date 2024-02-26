#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Object
{
protected:
	vector<Drawable*> drawables;

public:

	vector<Drawable*> GetDrawables() const
	{ 
		return drawables; 
	}	
	
	virtual void Update() = 0;
};

