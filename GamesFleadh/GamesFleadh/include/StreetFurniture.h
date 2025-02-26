#pragma once
#include "gameobject.h"
#include "Feeder.h"
#include "Globals.h"
#include <string>
#include "Player.h"
#include "reasings.h"
#include "State.h"
#include "IdleState.h"
#include <cmath> // For the pow() function
// #include <memory>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

class StreetFurniture :
    public GameObject
{
public:
	StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos = { 0.0f,0.0f,0.0f }, FurnitureType t_type = NONE);
	// StreetFurniture(StreetFurniture&&) = default;
	~StreetFurniture();

	void rotate(int t_direction) override;
	void init() override;
	void render() override;

	void renderBoom(Camera& t_camera);

	void playerDetected(bool t_spotted, Vector3 t_target);

	void update(Vector3 t_target);

	void spawnFeeder();

	void setHitBox();

	void setRelativePosition(Vector3 t_mapPos);

	bool checkPlayerFurnitureCollision(BoundingBox t_player);

	bool checkRadialFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRad);

	bool checkFeederBulletCollision(Vector3 t_bulletPos, float t_bulletRadius);

	bool checkMudbombPlayerCollision(BoundingBox t_player);

	void makeFeederSeekPlayer(bool t_seeking, Player player);

	void makeFeederEat();

	float exponentialScale(float scalar, float minimum, float maximum, float base);

private:
	//// Feeder* m_feeder{}; // Starts as null pointer
	//std::unique_ptr<Feeder> m_feeder;
	bool m_hasFeeder;
	Feeder m_feeder;

	FurnitureType m_type;

	Vector3 m_placementOffset = { 0.0f, 0.0f, 0.0f };
	float m_collisionRadiusMin = 1.5f;
	float m_collisionRadiusMax = 0.0f;
	float m_interpolatedColRadius = 0.0f;

	float m_overallHeight = 0.0f;
	float m_overallHeightOnGround = 0.0f;
	Vector3 m_posWithPlayerHeight = Vector3Zero();
	Vector3 m_posWPlyrHeightNorm = Vector3Zero();


	int m_colourDecrease;
	float m_colourVal;
	int eatTick;
	const int MAX_EAT_TICK;
};

