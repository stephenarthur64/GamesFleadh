#pragma once
#include "gameobject.h"
#include "IdleState.h"

class Swarmer :
    public GameObject
{
public:
	Swarmer();

	virtual void rotate(int t_direction);
	virtual void init();
	virtual void render();

	void update();
};

