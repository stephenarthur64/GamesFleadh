#include "Tile.h"

Tile::Tile(std::string t_heightMapAddress = "", std::string t_furnitureMapAddress = "", std::string t_tileModelAddress = "") 
{
	m_heightMap = LoadImage(t_heightMapAddress.c_str());
	m_furnitureMap = LoadImage(t_furnitureMapAddress.c_str());
	
	if (t_tileModelAddress == "")
	{// Do we have a pre-built model?
		// If no, generate model
		Mesh heightmapMesh = GenMeshHeightmap(m_heightMap, MAP_SIZE);
		m_body = LoadModelFromMesh(heightmapMesh);
        m_heightMapTex = LoadTextureFromImage(m_heightMap);
        m_body.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = m_heightMapTex; // Set map diffuse texture
	}
	else
	{	// If yes, load model
		m_body = LoadModel(t_tileModelAddress.c_str());
	}

	// m_furnitureVec = processFurnitureMap(m_furnitureMap);
    processFurnitureMap(m_furnitureMap);
}

Tile::~Tile(){}

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

	DrawModel(m_body, m_position, 1.0f, GREEN_HILL); // m_colour - object colour should be used here
	
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
}

/// <summary>
/// @brief Sets position for tile and furniture (see consts below)
/// </summary>
/// <param name="t_current"></param>
void Tile::tileIsCurrent(bool t_current)
{
	if (t_current)
	{
		m_position = MAP_POS_CURRENT;
		// Pass position to furniture here
	}
	else
	{
		m_position = MAP_POS_NEXT;
		// Pass position to furniture here
	}

    setInPlay(true);
}

/// <summary>
/// @brief Returns if collision is occuring at this heightmapPoint
/// </summary>
/// <param name="t_collider"></param>
/// <returns></returns>
bool Tile::collision(Vector3 t_collider)
{
	return false;
}

void Tile::update()
{
    for (StreetFurniture item : m_furnitureVec)
    {
        item.update();
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
	// std::vector<StreetFurniture> furn;

    // int furnitureCount = 0;

    for (int u = 0; u < t_furnitureMap.width; u++)
    {
        for (int v = 0; v < t_furnitureMap.height; v++)
        {
            Color col = GetImageColor(t_furnitureMap, u, v);
            
            if (col.a != 0) // Possibly early out here
            {
                std::cout << "\nFound an active pixel.\n";

                std::string furnType = FURNITURE_DEFAULT_MUSH;
                
                if (col.r == 255 && col.b == 0 && col.g == 0) furnType = FURNITURE_DEFAULT_MUSH;
                if (col.r == 0 && col.b == 255 && col.g == 0) furnType = FURNITURE_BATCH_MUSH;
                if (col.r == 0 && col.b == 0 && col.g == 255) furnType = FURNITURE_BUMPY_MUSH;
                if (col.r == 255 && col.b == 255 && col.g == 0) furnType = FURNITURE_CHUNKY_MUSH;
                if (col.r == 255 && col.b == 0 && col.g == 255) furnType = FURNITURE_POINTY_MUSH;
                // if (col.r == 0 && col.b == 255 && col.g == 255) furnType = FURNITURE_POINTY_MUSH;
                // if (col.r == 255 && col.b == 255 && col.g == 255) furnType = FURNITURE_POINTY_MUSH;

                assignFurniture(u, v, furnType);

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

	// return furn;
}

void Tile::assignFurniture(float t_u, float t_v, std::string t_furnitureType)
{
    Color heightFromCol = GetImageColor(m_heightMap, t_u, t_v);

    float placementTexUcoord = static_cast<float>(t_u);
    float placementColYcoord = static_cast<float>(heightFromCol.r);
    float placementTexVcoord = static_cast<float>(t_v);

    float placeWorldNormX = placementTexUcoord / m_heightMap.width;
    float placeWorldNormY = placementColYcoord / 255.0f; // Divide by max col value
    float placeWorldNormZ = placementTexVcoord / m_heightMap.height;

    float placeWorldCoordX = (placeWorldNormX * MAP_SIZE.x) - abs(m_position.x);
    float placeWorldCoordY = placeWorldNormY * MAP_SIZE.y;
    float placeWorldCoordZ = ((placeWorldNormZ * MAP_SIZE.y) - abs(m_position.z) - SEEMING_MAGICAL_Z_OFFSET); // Not sure I need the last offset for objects

    //objectPlacementTest = { placeWorldCoordX, placeWorldCoordY, placeWorldCoordZ };
    Vector3 furniturePos = { placeWorldCoordX, placeWorldCoordY, placeWorldCoordZ };



    StreetFurniture article(false, t_furnitureType);

    m_furnitureVec.push_back(article);
}
