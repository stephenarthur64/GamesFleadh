#pragma once
#include <raylib.h>
#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS     "playstation"

#include "Globals.h"
#include "Player.h"
#include "Enemy.h"

class Game
{
public:
	Game();
	~Game();
	void run();
	void init();
	void render();
	void update();
	void loadAssets();

	void inputControl();
	void gamepadInit();
	void gamepadUpdate();
	void gamepadControl();
	void checkCollisions(BoundingBox t_a, BoundingBox t_b);

private:
	Camera camera;
	Image heightmapImage;
	Texture2D heightmapTexture;
	Mesh heightmapMesh;
	Model heightmapModel;
	Vector3 mapPosition;
	Vector3 mapPosition2;
	Vector3 camPos;
	
	float camSpeed = 0.1f;

	int gamepad = 0;

	float leftStickDeadzoneX;
	float leftStickDeadzoneY;
	float rightStickDeadzoneX;
	float rightStickDeadzoneY;
	float leftTriggerDeadzone;
	float rightTriggerDeadzone;

	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftTrigger;
	float rightTrigger;

	Player player;
	Enemy enemy;
};

