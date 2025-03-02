#pragma once
#include<iostream>
#include <raylib.h>
#include "raymath.h"
#include "rlgl.h" // RS: Added for skybox
#include <vector>
#include "reasings.h"
#include <random>
#include <time.h>


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
#include "StreetFurniture.h"
#include "Swarmer.h"
#include "Tile.h"

enum class GameState{TITLE, GAMEPLAY, GAME_OVER};

class Game
{
public:
	Game();
	~Game();
	void run();
	void init();
	// void placeObjectsFromImage(Image placementMap);
	void render();
	void update();
	void loadAssets();
	void setupSkybox();

	void inputControl();
	void gamepadInit();

	void gameBegins();

	void gamepadUpdate();
	void checkCollisions();

	void mapMove();
	void cameraMove();
	void crosshairMove();

	void reboundZ(Vector3 t_impactPoint);

	void fogVisibility();
	void reduceFog();

private:
	GameState state;

	Camera camera;
	Camera camTopDown;

	Font gameFont;

	Image heightmapImage;
	Texture2D heightmapTexture;
	Mesh heightmapMesh;
	Model heightmapModel;

	Image imgPlacementTest;

	Vector3 mapPosition = { -32.0f, -0.0f, -64.0f };
	Vector3 mapPosition2 = { -32.0f, -0.0f, -128.0f };
	Vector3 camPos;
	Vector3 camTopDownPos;
	
	Texture2D healthBar;
	Texture2D fogBar;
	Texture2D fogGradient;
	Texture2D fogVignette;

	Rectangle gradientSource;
	Rectangle gradientDest;

	Color fogOpacity;

	FurnitureCollisionData m_collisionData;

	int fogTick = 0;
	float heightVal = 0;

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

	float keyboardX;
	float keyboardY;

	int score;
	int activeMap;
	int gameOverTick;
	int countdownRespawn;

	bool autoScroll{ false };

	const float playerZOffsetFromCamera = 5.0f;
	Player player;
	// Mushroom mushroom[2];
	// StreetFurniture streetF[5];
	Swarmer swarmer[MAX_SWARMERS];

	int maxStreetFurniture = 5; // Changed from const as this will be set by terrain;

	int maxSwarmer = 5;

	// RS: May need to remove MAX_MUSHROOMS and mushroomOnMap
	const int MAX_MUSHROOMS = 2;
	int mushroomOnMap = 0;

	Texture2D bill;
	Vector3 billPositionStatic;
	Rectangle source;
	Vector3 billUp;
	float distanceStatic;
	float distanceRotating;
	float rotation = 0.0f;
	Vector3 billPositionRotating = { 0.0f, 6.0f, 5.0f };
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

	Vector3 objectPlacementTest;

	BoundingBox heightMapBounds;

	Music bgm;

	std::vector<Tile> m_terrainTileCollection;

	bool m_gameIsBeginning = true;

	int m_tileCurrent = 0;

	int m_tileNext = 0;

	const Vector3 M_REBOUND_DIRECTION = { 0.0f, 0.0f, -1.0f };
	float m_reboundCounter = 0.0f;
	const float m_reboundCountMax = 0.125f; // 33f;
	const float m_reboundForce = 5.0f;

	const float oobCounterMax = 1.5f;
	float oobCounterCur = 0.0f;

	const float oobResetCounterMax = 0.33f;
	float oobResetCounterCur = 0.0f;


	float diffBetweenLimits = 0.0f;
};

