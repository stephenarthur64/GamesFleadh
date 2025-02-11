#pragma once
#include <raylib.h>
#include "raymath.h"
#define PS_ALIAS     "playstation"

#include "Input.h"
#include "Command.h"
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
	void checkCollisions(BoundingBox t_a, BoundingBox t_b);

	void mapMove();

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
	float camDirection;

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

	int score;
	int activeMap;

	Player player;
	Enemy enemy;
};

