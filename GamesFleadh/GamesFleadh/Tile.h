#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "gameobject.h"
#include "StreetFurniture.h"

class Tile : public GameObject
{
public:
	Tile(std::string t_heightMapAddress, std::string t_furnitureMapAddress, std::string t_tileModelAddress);
	~Tile();

	virtual void rotate(int direction);
	virtual void init(); 
	virtual void render();

	void setExists(bool t_exists);
	void tileIsCurrent(bool t_current); 

	bool collision(Vector3 t_collider);

private:
	Image m_heightMap;
	Image m_furnitureMap;
	// Model m_tileModel;
	std::vector<StreetFurniture> m_furnitureVec;

	int max_furniture = 0;

	const Vector3 MAP_SIZE = { 64, 64, 64 };
	const Vector3 MAP_POS_CURRENT = { -32.0f, -0.0f, -64.0f };
	const Vector3 MAP_POS_NEXT = { -32.0f, -0.0f, -128.0f };
	const float SEEMING_MAGICAL_Z_OFFSET = 2.0f;

	std::vector<StreetFurniture> processFurnitureMap(Image t_furnitureMap);
};

