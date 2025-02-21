#pragma once
//#include <stdlib.h>
#include <string>

enum Direction {NORTH, SOUTH, EAST, WEST};

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const Vector3 PLAYER_COLLISION_OFFSET_FRONT = Vector3{ 0.0f, -0.25f, -3.0f };
const Vector3 PLAYER_COLLISION_OFFSET_LATERAL = Vector3{ 2.0f,-0.25f,0.0f };


const Color GREEN_HILL = { 147, 204, 147, 255 }; // Green tint for generated terrain

const std::string GULLY_DIFFUSE_01 = "ASSETS/2D/GullyTextures/HeightmapPaint.png";
const std::string GULLY_DIFFUSE_01a ="ASSETS/2D/GullyTextures/revsTerribleColourGuide01001RS.png";


const std::string FURNITURE_BATCH_MUSH = "ASSETS/3D/StreetFurniture/Mushrooms/BatchMushroom.glb";
const std::string FURNITURE_BUMPY_MUSH = "ASSETS/3D/StreetFurniture/Mushrooms/BumpyMushroom.glb";
const std::string FURNITURE_CHUNKY_MUSH = "ASSETS/3D/StreetFurniture/Mushrooms/ChunkyMushroom.glb";
const std::string FURNITURE_DEFAULT_MUSH = "ASSETS/3D/StreetFurniture/Mushrooms/DefaultMushroom.glb";
const std::string FURNITURE_POINTY_MUSH = "ASSETS/3D/StreetFurniture/Mushrooms/PointyMushroom.glb";


const std::string ASSET_HEIGHTMAP_01 = "ASSETS/2D/Heightmaps/test1_3xWider_halfDark4_Rot_halfDark3.png";
const std::string ASSET_FURNITUREMAP_01 = "ASSETS/2D/Heightmaps/test1_EnemyPlacement01001RS.png";
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