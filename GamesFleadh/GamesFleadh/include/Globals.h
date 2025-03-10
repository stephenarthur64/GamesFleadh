#pragma once
//#include <stdlib.h>
#include <string>

enum Direction {NORTH, SOUTH, EAST, WEST};

enum FurnitureType {NONE, SWARMER, MUSHROOM, DEFAULT_MUSHROOM, BATCH_MUSHROOM, BUMPY_MUSHROOM, CHUNKY_MUSHROOM, POINTY_MUSHROOM, NOT_MUSHROOM}; // temp values

// RS: Way to pass col point from StreetFurniture.cpp back to Game radialFurnitureCollision func. Mea culpa.
typedef struct {
	Vector3 lastFurnitureCollision; 
	float lastFurnitureRadius;
	bool collision;
}FurnitureCollisionData;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const Vector3 PLAYER_COLLISION_OFFSET_FRONT = Vector3{ 0.0f, -0.25f, -3.0f };
const Vector3 PLAYER_COLLISION_OFFSET_LATERAL = Vector3{ 2.0f,-0.25f,0.0f };


const Color GREEN_HILL = { 147, 204, 147, 255 }; // Green tint for generated terrain

// const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/HeightmapPaint.png";
// const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/river_test6.png";
const std::string GULLY_DIFFUSE_RIVERTEST01 = "ASSETS/2D/GullyTextures/river_test6.png";


const std::string FURNITURE_DUMMY_ZERO = "If this dummy is found, ignore it!";

const std::string FURNITURE_BATCH_MUSH = "ASSETS/3D/StreetFurniture/Animated Mushrooms/BatchMushroomAnimated.glb";
const std::string FURNITURE_BUMPY_MUSH = "ASSETS/3D/StreetFurniture/Animated Mushrooms/BumpyMushroomAnimated.glb";
const std::string FURNITURE_CHUNKY_MUSH = "ASSETS/3D/StreetFurniture/Animated Mushrooms/ThickMushroomAnimated.glb";
const std::string FURNITURE_DEFAULT_MUSH = "ASSETS/3D/StreetFurniture/Animated Mushrooms/DefaultMushroomAnimated.glb";
const std::string FURNITURE_POINTY_MUSH = "ASSETS/3D/StreetFurniture/Animated Mushrooms/PointyMushroomAnimated.glb";

const std::string FURNITURE_STONE_LARGE = "ASSETS/3D/StreetFurniture/Stones/stoneLarge.glb";
const std::string FURNITURE_STONE_MED_FLAT01 = "ASSETS/3D/StreetFurniture/Stones/stoneMediumFlat.glb";
const std::string FURNITURE_STONE_MED_FLAT02 = "ASSETS/3D/StreetFurniture/Stones/stoneMediumFlatV2.glb";
const std::string FURNITURE_STONE_MED_POINTY = "ASSETS/3D/StreetFurniture/Stones/stoneMediumPointy.glb";
const std::string FURNITURE_STONE_SMALL01 = "ASSETS/3D/StreetFurniture/Stones/stoneSmallV1.glb";
const std::string FURNITURE_STONE_SMALL02 = "ASSETS/3D/StreetFurniture/Stones/stoneSmallV2.glb";

const std::string FURNITURE_GRASS = "ASSETS/3D/StreetFurniture/Grass/grass.glb";

const std::string FURNITURE_BATCH_MUSH_COL = "ASSETS/3D/StreetFurniture/CollisionModels/BatchMushroomCollider.glb";
const std::string FURNITURE_BUMPY_MUSH_COL = "ASSETS/3D/StreetFurniture/CollisionModels/BumpyMushroomCollider.glb";
const std::string FURNITURE_CHUNKY_MUSH_COL = "This is deliberately wrong so the collision doesn't load for the ChunkyMush"; // "ASSETS/3D/StreetFurniture/CollisionModels/ChunkyThickMushroomCollider.glb";
const std::string FURNITURE_DEFAULT_MUSH_COL = "ASSETS/3D/StreetFurniture/CollisionModels/DefaultMushroomCollider.glb";
const std::string FURNITURE_POINTY_MUSH_COL = "ASSETS/3D/StreetFurniture/CollisionModels/PointyMushroomCollider.glb";


const std::string ASSET_HEIGHTMAP_01 = "ASSETS/2D/Heightmaps/test1_3xWider_halfDark4_Rot_halfDark3_rebalanced01.png";
const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/river_test6.png";
const std::string ASSET_FURNITUREMAP_01 = "ASSETS/2D/Heightmaps/test1_EnemyPlacement02001RS.png";
const std::string ASSET_TILE_MODEL_01 = "";

const std::string ASSET_HEIGHTMAP_02 = "ASSETS/2D/Heightmaps/nadenTest1.png"; // nadenTest1rebalanced1.png";
const std::string GULLY_DIFFUSE_02 = "ASSETS/2D/GullyTextures/heightmapTexture1.png";
const std::string ASSET_FURNITUREMAP_02 = "ASSETS/2D/Heightmaps/furniturePlacement1.png";
const std::string ASSET_TILE_MODEL_02 = "";

const std::string ASSET_HEIGHTMAP_03 = "ASSETS/2D/Heightmaps/nadenTest2.png"; //nadenTest2rebalanced1.png";
const std::string GULLY_DIFFUSE_03 = "ASSETS/2D/GullyTextures/heightmapTexture2.png";
const std::string ASSET_FURNITUREMAP_03 = "ASSETS/2D/Heightmaps/furniturePlacement2.png";
const std::string ASSET_TILE_MODEL_03 = "";

const std::string ASSET_HEIGHTMAP_04 = "ASSETS/2D/Heightmaps/nadenTest3.png"; // nadenTest3rebalanced1.png";
const std::string GULLY_DIFFUSE_04 = "ASSETS/2D/GullyTextures/heightmapTexture3.png";
const std::string ASSET_FURNITUREMAP_04 = "ASSETS/2D/Heightmaps/furniturePlacement3.png";
const std::string ASSET_TILE_MODEL_04  = "";

static float mudBombPosition; // This is awful, I know, but there was no other way besides MAJOR refactoring

static Vector3 g_lastFurnitureCollision; // RS: Way to pass col point from StreetFurniture.cpp back to Game radialFurnitureCollision func. Mea culpa.
static float g_lastFurnitureRadius; // RS: See above. =(

static Vector3 g_furnCollisionItem;
static Vector3 g_furnCollisionPlyr;

const float FURNITURE_TEST_OUTER_RADIUS = 8.0f;

extern bool g_renderWireDebug;

extern bool g_render2DDebug;

extern bool g_testForMushrooms;

const int MAX_SWARMERS = 5;