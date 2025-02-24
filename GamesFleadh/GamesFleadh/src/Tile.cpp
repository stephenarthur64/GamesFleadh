#include "Tile.h"

Tile::Tile(std::string t_heightMapAddress = "", std::string t_furnitureMapAddress = "", 
    std::string t_tileModelAddress = "", std::string t_diffuseMapAddress = "")
{
	m_heightMapImage    = LoadImage(t_heightMapAddress.c_str());
	m_furnitureMapImage = LoadImage(t_furnitureMapAddress.c_str());
    m_diffuseMapImage   = LoadImage(t_diffuseMapAddress.c_str()); // This is hardcoded.
	
	if (t_tileModelAddress == "")// Do we have a pre-built model?
	{// If no, generate model
		Mesh heightmapMesh = GenMeshHeightmap(m_heightMapImage, MAP_SIZE);
		m_body = LoadModelFromMesh(heightmapMesh);
	}
	else
	{	// If yes, load model
		m_body = LoadModel(t_tileModelAddress.c_str());
	}
    m_textureMapDiffuse = LoadTextureFromImage(m_diffuseMapImage); // Convert image to texture (VRAM)
    m_body.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_textureMapDiffuse; // Set map diffuse texture

    processFurnitureMap(m_furnitureMapImage);
}

Tile::~Tile(){} // Unloading images and meshes - take from Game

void Tile::rotate(int direction){}

/// <summary>
/// @brief Parses heightMap and furnitureMap, determines if must generate model
/// </summary>
void Tile::init(){} // Turns out most of this is done in the constructor. =/

/// <summary>
/// @brief Make tile and furniture visible
/// </summary>
void Tile::render()
{
	if (!m_inPlay) return;
    // GREEN_HILL); // m_colour - object colour should be used here
    DrawModel(m_body, m_position, 1.0f, WHITE);
    // Render furniture?

    for (StreetFurniture item : m_furnitureVec)
    {
        item.render();
    }
}

/// <summary>
/// @brief On/Off. Should also set associated furniture 'existance'
/// </summary>
/// <param name="t_inPlay"></param>
void Tile::setInPlay(bool t_inPlay)
{
	m_inPlay = t_inPlay;

    for (StreetFurniture& item : m_furnitureVec)
    {
        //std::cout << "Setting item to: " << t_inPlay << "\n";
        item.m_inPlay = t_inPlay;
    }
}

/// <summary>
/// @brief Sets position for tile and furniture (see consts below)
/// </summary>
/// <param name="t_current"></param>
void Tile::makeTileCurrent(bool t_current)
{
	if (t_current)
	{
		m_position = MAP_POS_CURRENT;

        for (StreetFurniture& item : m_furnitureVec)
        {// Pass position to furniture here
            item.setRelativePosition(MAP_POS_CURRENT);
        }
	}
	else
	{
		m_position = MAP_POS_NEXT;
		
        for (StreetFurniture& item : m_furnitureVec)
        {// Pass position to furniture here
            item.setRelativePosition(MAP_POS_NEXT);
        }
	}
    setInPlay(true);
}

/// <summary>
/// @brief Returns true if collision is occuring at this heightMap position
/// </summary>
/// <param name="t_collider"></param>
/// <returns></returns>
bool Tile::isColliding(Vector3 t_collider)
{
    m_worldNormalX = (t_collider.x + abs(MAP_POS_CURRENT.x)) / MAP_SIZE.x; // Get Normalised Coord
    m_worldNormalZ = ((t_collider.z + SEEMING_MAGICAL_Z_OFFSET) + abs(MAP_POS_CURRENT.z)) / MAP_SIZE.z;
    m_texUcoord = m_worldNormalX * m_heightMapImage.width;
    m_texVcoord = m_worldNormalZ * m_heightMapImage.height;

    m_texUcoord = Clamp(m_texUcoord, 0, m_heightMapImage.height - 0.001f); // Avoids OOBounds error
    m_texVcoord = Clamp(m_texVcoord, 0, m_heightMapImage.width - 0.001f);

    m_colorFromPosition = GetImageColor(m_heightMapImage, m_texUcoord, m_texVcoord);
    m_worldYNormalFromCol = m_colorFromPosition.r / 255.0f;
    m_worldYPos = m_worldYNormalFromCol * MAP_SIZE.y;

    if (t_collider.y <= m_worldYPos)
    {
        return true;
    }
    else
    {
        return false;
    }
	return false;
}

bool Tile::checkFurnitureItemsCollision(BoundingBox t_player)
{
    for (StreetFurniture& item : m_furnitureVec)
    {        
        if (item.checkPlayerFurnitureCollision(t_player))
        {
            return true;
        }
    }
    return false;
}

bool Tile::checkFeederBulletCollision(Vector3 t_bulletPos, float t_bulletRadius)
{
    for (StreetFurniture& item : m_furnitureVec)
    {
        if (item.checkFeederBulletCollision(t_bulletPos, t_bulletRadius))
        {
            return true;
        }
    }
    return false;
}

bool Tile::checkMudBombPlayerCollision(BoundingBox t_player)
{
    for (StreetFurniture& item : m_furnitureVec)
    {
        if (item.checkMudbombPlayerCollision(t_player))
        {
            return true;
        }
    }
    return false;
}

void Tile::makeFeederSeekPlayer(bool t_seeking, Player player)
{
    for (StreetFurniture& item : m_furnitureVec)
    {
        item.makeFeederSeekPlayer(t_seeking, player);
    }
}

void Tile::update(Vector3 t_target)
{
    for (StreetFurniture& item : m_furnitureVec)
    {
        item.update(t_target);
        item.makeFeederEat();
    }
}

/// <summary>
/// @brief Reads through image, identifies types of furniture and places them in level
/// </summary>
/// <param name="t_furnitureMap"></param>
/// <returns></returns>
// std::vector<StreetFurniture> Tile::processFurnitureMap(Image t_furnitureMap)
void Tile::processFurnitureMap(Image t_furnitureMap)
{
    for (int u = 0; u < t_furnitureMap.width; u++)
    {
        for (int v = 0; v < t_furnitureMap.height; v++)
        {
            Color col = GetImageColor(t_furnitureMap, u, v);
            
            if (col.a != 0) // Possibly early out here
            {
                std::cout << "\nFound an active pixel.\n";

                std::string furnType = FURNITURE_DUMMY_ZERO;
                
                if (col.r == 255 && col.b == 0 && col.g == 0) furnType = FURNITURE_DEFAULT_MUSH;
                if (col.r == 0 && col.b == 255 && col.g == 0) furnType = FURNITURE_BATCH_MUSH;
                if (col.r == 0 && col.b == 0 && col.g == 255) furnType = FURNITURE_BUMPY_MUSH;
                if (col.r == 255 && col.b == 255 && col.g == 0) furnType = FURNITURE_CHUNKY_MUSH;
                if (col.r == 255 && col.b == 0 && col.g == 255) furnType = FURNITURE_POINTY_MUSH;
                
                if (col.r == 0 && col.b == 255 && col.g == 255) furnType = FURNITURE_GRASS;
                
                if (col.r == 5 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_SMALL01;
                if (col.r == 10 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_SMALL01;
                if (col.r == 100 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_MED_FLAT01;
                if (col.r == 100 && col.b == 0 && col.g == 0)
                {
                    std::cout << "!!!!!!!!!!!!Found r15!!!!!!!!!!!!\n!!!!!!!!!!!!Found r15!!!!!!!!!!!!\n!!!!!!!!!!!!Found r15!!!!!!!!!!!!\n";
                }
                if (col.r == 20 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_MED_FLAT02;
                if (col.r == 25 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_MED_POINTY;
                if (col.r == 30 && col.b == 0 && col.g == 0) furnType = FURNITURE_STONE_LARGE;

                if(furnType != FURNITURE_DUMMY_ZERO) assignFurniture(u, v, furnType);

                //if (furnitureCount < max_furniture)
                //{
                //    m_furnitureVec[furnitureCount].init();

                //    //m_furnitureVec[furnitureCount].spawn(furniturePos);

                //    //m_furnitureVec[furnitureCount].spawnEnemy();
                //    
                //    furnitureCount++;
                //}

                //m_furnitureVec[0].playerDetected(true);

                ///*for (int i = 0; i < MAX_MUSHROOMS; i++)
                //{
                //    mushroom[i].init();
                //    if (i != mushroomOnMap)
                //    {
                //        mushroom[i].spawn({ -1.0f, 2.0f, -79.0f });
                //    }
                //    mushroom[i].spawnEnemy();
                //}*/

            }
        }
    }
}

void Tile::assignFurniture(float t_u, float t_v, std::string t_furnitureType)
{
    Color heightFromCol = GetImageColor(m_heightMapImage, t_u, t_v);

    float placementTexUcoord = static_cast<float>(t_u);
    float placementColYcoord = static_cast<float>(heightFromCol.r);
    float placementTexVcoord = static_cast<float>(t_v);

    float placeWorldNormX = placementTexUcoord / m_heightMapImage.width;
    float placeWorldNormY = placementColYcoord / 255.0f; // Divide by max col value
    float placeWorldNormZ = placementTexVcoord / m_heightMapImage.height;

    float placeWorldCoordX = (placeWorldNormX * MAP_SIZE.x) - abs(m_position.x);
    float placeWorldCoordY = placeWorldNormY * MAP_SIZE.y;
    float placeWorldCoordZ = ((placeWorldNormZ * MAP_SIZE.y) - abs(m_position.z) - SEEMING_MAGICAL_Z_OFFSET); // Not sure I need the last offset for objects

    Vector3 furniturePos = { placeWorldCoordX, placeWorldCoordY, placeWorldCoordZ };

    int randomChance = rand() % 3;

    StreetFurniture article(randomChance != 0, t_furnitureType, furniturePos);

    // StreetFurniture article(true, t_furnitureType, furniturePos); // SWITCH BACK TO TRUE FOR FIXING

    m_furnitureVec.push_back(article);
}
