#pragma once
#include<iostream>
#include <raylib.h>
#include "raymath.h"
#include "rlgl.h" // RS: Added for skybox
#include <vector>
#include "reasings.h"
#include <random>
#include <time.h>
#include "AchievementManager.h"

#include <SDL.h> // RUMBLE REQUIREMENT


// RS: Again, added for skybox - shader for skybox depends on platform.
#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define PS_ALIAS     "playstation"

#define FLT_MAX     340282346638528859811704183484516925440.0f     // Maximum value of a float, from bit pattern 01111111011111111111111111111111

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

	void fogVisibility();
	void reduceFog();
	void darkenScreenUpdate();
	void healthBarUpdate();

	void Rumble(SDL_GameController* controller, Uint16 lowFreq, Uint16 highFreq, Uint32 duration); // RUMBLE REQUIREMENT
	// void StartRumble(Uint16 lowFrequency, Uint16 highFrequency, Uint32 duration);
	void UpdateRumble(SDL_GameController* controller);



	void painVignetteStart();
	void painCountDown();

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
	Texture2D healthGradient;
	Texture2D fogBar;
	Texture2D fogGradient;
	Texture2D fogVignette;
	Texture2D scoreBack;

	Texture2D countdown[3];
	Texture2D countdownText;
	Texture2D darkenScreen;
	Color darkenColour;

	Texture2D logo;
	Texture2D subtitle;
	Texture2D controllerInputs;
	Texture2D keyboardInputs;
	Texture2D arrow;
	Texture2D arrow2;
	Texture2D difficulty[3];
	Texture2D leave;

	Rectangle gradientSource;
	Rectangle gradientDest;
	Rectangle healthSource;
	Rectangle healthDest;

	Color fogOpacity;

	FurnitureCollisionData m_collisionData;

	int fogTick = 0;
	float heightVal = 0;

	int selectedDifficulty = 0;

	int healthTick = 0;
	float heightHealth = 0;

	float camSpeed = 0.2f;
	float camDirection;

	float arrowYOffset = 130.0f;
	bool difficultySwitch = false;

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

	bool autoScroll{ true };

	float m_painCounterMax = 0.33f;
	float m_painCounterCur = 0.0f;
	int m_painAlphaMax = 128;
	int m_painAlphaCur = 0.0f;
	bool m_painShow = false;

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
	bool m_bgm_toggle = true;
	Music titleScreenTrack;
	Sound sfxHover;
	Sound sfxSelect;

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

	bool endGame = false;

	float diffBetweenLimits = 0.0f;

	Vector2 m_lowerLimit;
	Vector2 m_upperLimit;

	Ray m_ray = { 0 };
	Vector2 m_crosshairOnScreenPos = Vector2Zero();
	RayCollision m_collision = { 0 };

	AchievementManager achievementManager;

	int travelled = 0;

	SDL_GameController* controller; // RUMBLE REQUIREMENT

	Uint32 m_rumbleStartTimeRS = 0;
	Uint32 m_rumbleDurationRS = 0; // 250ms duration
	bool m_isRumbling = false;
	Uint32 m_rumbleCooldown = 200;  // Cooldown between rumbles (adjust as needed)

	float m_keyboardLookSensitivity = 2.0f;
	float m_keyboardMoveSensitivity = 1.0f;
	float m_controllerLookSensitivity = 1.0f;
	float m_controllerMoveSensitivity = 1.0f;

	bool m_showPainVignette = false;
};

