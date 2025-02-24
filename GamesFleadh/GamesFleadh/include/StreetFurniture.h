#pragma once
#include "gameobject.h"
#include "Feeder.h"
#include "Globals.h"
#include <string>
#include "Player.h"
// #include <memory>

class StreetFurniture :
    public GameObject
{
public:
	StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos = { 0.0f,0.0f,0.0f });
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

	bool checkFeederBulletCollision(Vector3 t_bulletPos, float t_bulletRadius);

	bool checkMudbombPlayerCollision(BoundingBox t_player);

	void makeFeederSeekPlayer(bool t_seeking, Player player);

	void makeFeederEat();

private:
	//// Feeder* m_feeder{}; // Starts as null pointer
	//std::unique_ptr<Feeder> m_feeder;
	bool m_hasFeeder;
	Feeder m_feeder;

	Vector3 m_placementOffset = { 0.0f, 0.0f, 0.0f };
	int m_colourDecrease;
	float m_colourVal;
	int eatTick;
	const int MAX_EAT_TICK;
};

