#pragma once
#include "gameobject.h"
#include "Enemy.h"
#include "Globals.h"
#include <string>

class StreetFurniture :
    public GameObject
{
public:
	StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos = { 0.0f,0.0f,0.0f });
	~StreetFurniture();

	void rotate(int t_direction) override;
	void init() override;
	void render() override;

	void renderBoom(Camera& t_camera);

	void playerDetected(bool t_spotted);

	void update();

	void spawnEnemy();

	void setHitBox();

private:
	Enemy* m_feeder{}; // Starts as null pointer
};

