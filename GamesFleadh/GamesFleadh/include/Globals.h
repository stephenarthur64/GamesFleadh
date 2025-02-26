#pragma once
//#include <stdlib.h>
#include <string>

enum Direction {NORTH, SOUTH, EAST, WEST};

enum FurnitureType {NONE, MUSHROOM, NOT_MUSHROOM}; // temp values

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const Vector3 PLAYER_COLLISION_OFFSET_FRONT = Vector3{ 0.0f, -0.25f, -3.0f };
const Vector3 PLAYER_COLLISION_OFFSET_LATERAL = Vector3{ 2.0f,-0.25f,0.0f };


const Color GREEN_HILL = { 147, 204, 147, 255 }; // Green tint for generated terrain

// const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/HeightmapPaint.png";
const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/river_test6.png";
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

const std::string FURNITURE_GRASS = "ASSETS/3D/StreetFurniture/Grass/grassMushroom.glb";


const std::string ASSET_HEIGHTMAP_01 = "ASSETS/2D/Heightmaps/test1_3xWider_halfDark4_Rot_halfDark3.png";
const std::string ASSET_FURNITUREMAP_01 = "ASSETS/2D/Heightmaps/test1_EnemyPlacement02001RS.png";
const std::string ASSET_TILE_MODEL_01 = "";

const std::string ASSET_HEIGHTMAP_02 = "";
const std::string ASSET_FURNITUREMAP_02 = "";
const std::string ASSET_TILE_MODEL_02 = "";

const std::string ASSET_HEIGHTMAP_03 = "";
const std::string ASSET_FURNITUREMAP_03 = "";
const std::string ASSET_TILE_MODEL_03 = "";

const std::string ASSET_HEIGHTMAP_04 = "";
const std::string ASSET_FURNITUREMAP_04 = "";
const std::string ASSET_TILE_MODEL_04  = "";

static float mudBombPosition; // This is awful, I know, but there was no other way besides MAJOR refactoring