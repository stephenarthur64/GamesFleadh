#pragma once
#include <raylib.h>
#include "Globals.h"

class Game
{
public:
	Game();
	~Game();
	void run();
	void init();
	void render();
	void update();

	void inputControl(float& t_camPos);

private:
	Camera camera;
	Image heightmapImage;
	Texture2D heightmapTexture;
	Mesh heightmapMesh;
	Model heightmapModel;
	Vector3 mapPosition;
	Vector3 mapPosition2;
	float newCamX = 8.0f;
};

