#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "gameobject.h"
#include "StreetFurniture.h"
#include "Globals.h"

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
	void tileIsCurrent(bool t_current); 

	bool isColliding(Vector3 t_collider);
	bool checkFurnitureItemsCollision(BoundingBox t_player);

	std::vector<StreetFurniture> getFurniture() { return m_furnitureVec; } // For debugging

	void update(Vector3 t_target);

private:
	Image m_heightMapImage;
	Texture2D m_textureMapDiffuse;
	Mesh m_heightMapMesh;
	Image m_furnitureMapImage;
	Image m_diffuseMapImage;
	// Model m_tileModel;
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

	// std::vector<StreetFurniture> processFurnitureMap(Image t_furnitureMap);
	void processFurnitureMap(Image t_furnitureMap);

	void assignFurniture(float t_u, float t_v, std::string t_furnitureType);
};

