#pragma once
#include<iostream>
#include <raylib.h>
#include "raymath.h"
#include "rlgl.h" // RS: Added for skybox

// RS: Again, added for skybox - shader for skybox depends on platform.
#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define PS_ALIAS     "playstation"

#include "Input.h"
#include "Command.h"
#include "Globals.h"
#include "Player.h"
#include "Mushroom.h"

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
	void setupSkybox();

	void inputControl();
	void gamepadInit();
	void gamepadUpdate();
	void checkCollisions(BoundingBox t_a, BoundingBox t_b);

	void mapMove();
	void cameraMove();

private:
	Camera camera;
	Camera camTopDown;

	Image heightmapImage;
	Texture2D heightmapTexture;
	Mesh heightmapMesh;
	Model heightmapModel;

	Image heightmapImageTest;
	Texture2D heightmapTextureTest;
	Mesh heightmapMeshTest;
	Model heightmapModelTest;

	Vector3 mapPosition = { -32.0f, -0.0f, -64.0f };
	Vector3 mapPosition2 = { -32.0f, -0.0f, -128.0f };
	Vector3 camPos;
	Vector3 camTopDownPos;
	
	float camSpeed = 0.2f;
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

	bool autoScroll{ false };

	const float playerZOffsetFromCamera = 5.0f;
	Player player;
	Mushroom mushroom[2];

	Vector2 lowerLimit = { 4.0f, 0.0f };
	Vector2 upperLimit = { 8.0f, 3.0f };

	const int MAX_MUSHROOMS = 2;
	int mushroomOnMap = 0;

	Texture2D bill;
	Vector3 billPositionStatic;
	Rectangle source;
	Vector3 billUp;
	float distanceStatic;
	float distanceRotating;
	float rotation = 0.0f;
	Vector3 billPositionRotating = { 1.0f, 2.0f, 1.0f };
	Vector2 size;
	Vector2 origin;
	float billSpeed = 0.0f;

	// Skybox variables
	Mesh cube;
	Model skybox;

	// Terrain Collision Variables
	const float SeemingMagicalOffset = 2.0f;

	const Vector3 mapSize = { 64, 64, 64 };

	float worldNormalX;
	float worldNormalZ;
	float texUcoord;
	float texVcoord;

	Color colorFromPosition;
	float worldYNormalFromCol;
	float worldYPos;

	BoundingBox heightMapBounds;
};

