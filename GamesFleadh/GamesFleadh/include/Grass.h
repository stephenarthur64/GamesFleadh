#pragma once
#include "gameobject.h"
#include "Globals.h"
#include "IdleState.h"

class Grass :
    public GameObject
{
public:
	Grass();
	virtual void rotate(int t_direction) override {};
	virtual void init() override;
	virtual void render() override;
	void update();

	void spawn(Vector3 t_pos);
};

