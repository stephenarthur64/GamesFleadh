#pragma once
#include "gameobject.h"
class StreetFurniture :
    public GameObject
{
public:
	StreetFurniture();

	virtual void rotate(int t_direction);
	virtual void init();
	virtual void render();

	void setHitBox();
};

