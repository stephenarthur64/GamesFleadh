#pragma once
#include <string>
#include <vector>
#include <iostream>
//#include <memory>
#include "gameobject.h"
#include "StreetFurniture.h"
#include "Globals.h"
#include "Player.h"

class Tile : public GameObject
{
public:
	Tile(std::string t_heightMapAddress, std::string t_furnitureMapAddress,
		std::string t_tileModelAddress, std::string t_diffuseMapAddress);
	~Tile();

	void rotate(int direction) override;
	void init() override; 
	void render() override;

	void setInPlay(bool t_exists);
	void makeTileCurrent(bool t_current);

	// void makeTileFeedersActive()

	bool isColliding(Vector3 t_collider);
	bool checkFurnitureItemsCollision(BoundingBox t_player);
	// bool checkRadialFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRad);
	bool checkBoundsFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRadius, BoundingBox t_playerBox);

	bool checkFeederBulletCollision(Vector3 t_bulletPos, float t_bulletRadius);

	bool checkMudBombPlayerCollision(BoundingBox t_player);

	void makeFeederSeekPlayer(bool t_seeking, Player player);

	Vector3 getSwarmerPos(int index);

	// std::vector<StreetFurniture> getFurniture() { return m_furnitureVec; } // For debugging

	void update(Vector3 t_target);

private:
	Image m_heightMapImage;
	Texture2D m_textureMapDiffuse;
	Mesh m_heightMapMesh;
	Image m_furnitureMapImage;
	Image m_diffuseMapImage;
	// Model m_tileModel;

	Vector3 m_swarmerPos[MAX_SWARMERS];
	int m_swarmerPosCount = 0;
	
	FurnitureCollisionData m_data;

	std::vector<StreetFurniture> m_furnitureVec;

	float m_worldNormalX;
	float m_worldNormalZ;
	float m_texUcoord;
	float m_texVcoord;

	Color m_colorFromPosition;
	float m_worldYNormalFromCol;
	float m_worldYPos;

	int max_furniture = 0;

	const Vector3 MAP_SIZE = { 64, 64, 64 };
	const Vector3 MAP_POS_CURRENT = { -32.0f, -0.0f, -64.0f };
	const Vector3 MAP_POS_NEXT = { -32.0f, -0.0f, -128.0f };
	const float SEEMING_MAGICAL_Z_OFFSET = 2.0f;

	void processFurnitureMap(Image t_furnitureMap);

	void assignFurniture(float t_u, float t_v, std::string t_furnitureType, FurnTypeEnum t_type);


};

