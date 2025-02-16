#pragma once
#include "gameobject.h"
#include "IdleState.h"

class Weapon :
    public GameObject
{
public:
	Weapon();

	virtual void rotate(int t_direction) override;
	virtual void init() override;
	virtual void render() override;


	void update(Vector3 t_newPos);
};

