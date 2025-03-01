#include "StreetFurniture.h"
#include <iostream>

StreetFurniture::StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos, FurnTypeEnum t_typeEnum, bool t_hasCollider) : 
																															m_hasFeeder(t_hasFeeder),
																															m_colourDecrease(0),
																															m_colourVal(255), eatTick(0), 
																															MAX_EAT_TICK(60),
																															m_typeEnum(t_typeEnum),
																															m_hasCollider(t_hasCollider)
																									
{
	m_placementOffset = t_startPos;
	currentState = new IdleState;
	m_body = LoadModel(t_furnitureType.c_str());
	
	
	for (int i = 0; i < m_body.meshCount; i++)
	{
		BoundingBox bokz = GetMeshBoundingBox(m_body.meshes[i]);

		if (bokz.max.y > m_highestPoint)
		{
			m_highestPoint = bokz.max.y;
		}
	}



	/*for (int i = 0; i < m_body.materialCount; i++)
	{
		m_body.materials[i].shader = LoadShader(TextFormat("ASSETS/shaders/glsl%i/discard_alpha.fs", GLSL_VERSION),
			TextFormat("ASSETS/shaders/glsl%i/discard_alpha.fs", GLSL_VERSION));
	}*/
	// setHitBox();	

	// enum FurnitureType { NONE, MUSHROOM, CHUNKY_MUSHROOM, POINTY_MUSHROOM, NOT_MUSHROOM }; // temp values

	std::string furnType = "";

	switch (m_typeEnum)
	{
	case NONE:
		furnType = "NONE";
		break;
	case DEFAULT_MUSHROOM:
		furnType = "DEFAULT_MUSHROOM";
		m_collider = LoadModel(FURNITURE_DEFAULT_MUSH_COL.c_str());
		m_hasCollider = true;
		break;
	case BATCH_MUSHROOM:
		furnType = "BATCH_MUSHROOM";
		m_collider = LoadModel(FURNITURE_BATCH_MUSH_COL.c_str());
		m_hasCollider = true;
		break;
	case BUMPY_MUSHROOM:
		furnType = "BUMPY_MUSHROOM";
		m_collider = LoadModel(FURNITURE_BUMPY_MUSH_COL.c_str());
		m_hasCollider = true;
		break;
	case CHUNKY_MUSHROOM:
		furnType = "CHUNKY_MUSHROOM";
		m_collider = LoadModel(FURNITURE_CHUNKY_MUSH_COL.c_str());
		m_hasCollider = true;
		break;
	case POINTY_MUSHROOM:
		furnType = "POINTY_MUSHROOM";
		m_collider = LoadModel(FURNITURE_POINTY_MUSH_COL.c_str());
		m_hasCollider = true;
		break;
	case NOT_MUSHROOM:
		furnType = "NOT_MUSHROOM";
		break;
	default:
		break;
	}

	if (m_hasCollider)
	{
		for (int i = 0; i < m_collider.meshCount; i++)
		{
			BoundingBox collider = GetMeshBoundingBox(m_collider.meshes[i]);

			m_modelBoundingBoxes.push_back(collider);
		}
	}

	std::cout << "\n=================================================================\n";
	std::cout << "Collision Detection, list meshes in Furniture Type " << furnType << ":\n";

	for (int i = 0; i < m_body.materialCount; i++)
	{
		std::cout << "Mesh #" << i << "\n";
	}
	std::cout << "\n=================================================================\n";

	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations(t_furnitureType.c_str(), &animsCount);
	if (m_typeEnum == POINTY_MUSHROOM)
	{
		m_hasFeeder = false;
	}

	if (m_hasFeeder)
	{
		m_feeder.init();
	}
	m_health = 255;

	initStones();
	m_grass = LoadModel(FURNITURE_GRASS.c_str());
}

StreetFurniture::~StreetFurniture()
{
	/*if (m_feeder == nullptr) return;
	delete m_feeder;*/
}

void StreetFurniture::rotate(int t_direction){} // RS: Okay, so let's not rotate, maybe, and we'll get some nice collisions instead.

void StreetFurniture::init()
{
}

void StreetFurniture::render()
{
	if (!m_inPlay) return; // Not in gameplay: early out.

	DrawModel(m_body, m_position, 1.0f, m_colour);
	
	


	
	
	// DrawCylinderWires(m_position, m_collisionRadiusMin, m_collisionRadiusMin, 100.0f, 6, GREEN);

	//DrawCircle3D(m_posWithPlayerHeight, m_interpolatedColRadius, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, ORANGE);

	DrawCircle3D(m_position + Vector3{ 0.0f, 2.0f, 0.0f }, FURNITURE_TEST_OUTER_RADIUS, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, RED);

	for (int i = 0; i < 3; i++)
	{
		DrawModel(m_stones[i].body, m_stones[i].position, 0.5f, WHITE);
	}
	if (m_typeEnum != CHUNKY_MUSHROOM)
	{
		DrawModel(m_grass, m_grassPos, 0.8f, WHITE);
	}

	if (m_hasCollider)
	{
		Color boxCol;
		for (int i = 0; i < m_collider.meshCount; i++) // Goes through the collider MODEL, sets a specific colour
		{
			switch (i + 2)
			{
			case 0:
				boxCol = MAROON;
				break;
			case 1:
				boxCol = ORANGE;
				break;
			case 2:
				boxCol = DARKGREEN;
				break;
			case 3:
				boxCol = DARKBLUE;
				break;
			case 4:
				boxCol = DARKPURPLE;
				break;
			case 5:
				boxCol = DARKBROWN;
				break;
			case 6:
				boxCol = RED;
				break;
			case 7:
				boxCol = MAGENTA;
				break;
			case 8:
				boxCol = LIME;
				break;
			case 9:
				boxCol = BLUE;
				break;
			case 10:
				boxCol = VIOLET;
				break;
			case 11:
				boxCol = BROWN;
				break;
			case 12:
				boxCol = PINK;
				break;
			case 13:
				boxCol = YELLOW;
				break;
			case 14:
				boxCol = GREEN;
				break;
			case 15:
				boxCol = SKYBLUE;
				break;
			default:
				break;
			}

			DrawBoundingBox(m_modelBoundingBoxes[i], boxCol);
		}
	}

	if (!m_hasFeeder) return;
	m_feeder.render();
}

void StreetFurniture::initStones()
{
	if (m_typeEnum == CHUNKY_MUSHROOM) // Big mushroom needs large stones
	{
		for (int i = 0; i < 3; i++)
		{
			m_stones[i].body = LoadModel(FURNITURE_STONE_LARGE.c_str());
		}
		return;
	}

	int randNum;

	for (int i = 0; i < 3; i++)
	{
		randNum = rand() % 3;

		switch (randNum)
		{
		case 0:
			m_stones[i].body = LoadModel(FURNITURE_STONE_MED_POINTY.c_str());
			break;
		case 1:
			m_stones[i].body = LoadModel(FURNITURE_STONE_MED_FLAT01.c_str());
			break;
		case 2:
			m_stones[i].body = LoadModel(FURNITURE_STONE_SMALL01.c_str());
			break;
		default:
			break;
		}
		
	}
}

void StreetFurniture::renderBoom(Camera& t_camera)
{
	if (!m_hasFeeder) return;
	m_feeder.renderBoom(t_camera);
}

void StreetFurniture::playerDetected(bool t_spotted, Vector3 t_target)
{
	if (!m_hasFeeder) return;

	if (t_spotted)
	{
		m_feeder.shootBullet(t_target);
	}
	else
	{
		m_feeder.disableShooting();
	}
}

void StreetFurniture::update(Vector3 t_target)
{
	if (m_typeEnum == DEFAULT_MUSHROOM ||m_typeEnum == BATCH_MUSHROOM||m_typeEnum == BUMPY_MUSHROOM|| m_typeEnum == CHUNKY_MUSHROOM)
	{
		currentState->update(this);
	}

	if (!m_hasFeeder) return;
	m_feeder.update(t_target);

	if (!m_feeder.isAlive())
	{
		handleInput(EVENT_MOVE);
	}
}

void StreetFurniture::spawnFeeder()
{// Sets feeder health to 1, sets position, sets hitbox
	/*if (!m_hasFeeder) return;
	m_feeder.spawn(m_position); */
}

void StreetFurniture::setHitBox()
{
	/*m_hitbox.min.x = m_position.x - 1.0f; // Original code here!
	m_hitbox.max.x = m_position.x + 1.0f;

	m_hitbox.min.y = m_position.y - 2.5;
	m_hitbox.max.y = m_position.y + 9.5f;

	m_hitbox.min.z = m_position.z - 3.0f;
	m_hitbox.max.z = m_position.z - 1.0f;*/

	if (m_hasCollider)
	{
		for (int i = 0; i < m_collider.meshCount; i++)
		{
			m_modelBoundingBoxes[i].min.x = GetMeshBoundingBox(m_collider.meshes[i]).min.x + m_position.x;
			m_modelBoundingBoxes[i].max.x = GetMeshBoundingBox(m_collider.meshes[i]).max.x + m_position.x;

			m_modelBoundingBoxes[i].min.y = GetMeshBoundingBox(m_collider.meshes[i]).min.y + m_position.y;
			m_modelBoundingBoxes[i].max.y = GetMeshBoundingBox(m_collider.meshes[i]).max.y + m_position.y;

			m_modelBoundingBoxes[i].min.z = GetMeshBoundingBox(m_collider.meshes[i]).min.z + m_position.z;
			m_modelBoundingBoxes[i].max.z = GetMeshBoundingBox(m_collider.meshes[i]).max.z + m_position.z;
		}
	}

	BoundingBox localSpaceBoundingBox = GetMeshBoundingBox(m_body.meshes[0]);

	m_hitbox.min.x = localSpaceBoundingBox.min.x + m_position.x;
	m_hitbox.max.x = localSpaceBoundingBox.max.x + m_position.x;

	m_hitbox.min.y = localSpaceBoundingBox.min.y + m_position.y;
	m_hitbox.max.y = localSpaceBoundingBox.max.y + m_position.y;

	m_hitbox.min.z = localSpaceBoundingBox.min.z + m_position.z;
	m_hitbox.max.z = localSpaceBoundingBox.max.z + m_position.z;

	m_overallHeight = m_hitbox.max.y - m_hitbox.min.y;
	m_overallHeightOnGround = m_overallHeight + m_position.y;

	Vector3 flatMinPos = localSpaceBoundingBox.min;
	flatMinPos.y = 0.0f;

	Vector3 flatMaxPos = localSpaceBoundingBox.max;
	flatMaxPos.y = 0.0f;

	Vector3 centrePoint = (flatMinPos + flatMaxPos) * 0.5f;

	// m_collisionRadiusMax = Vector3Length(centrePoint - flatMinPos);

	m_collisionRadiusMax = (std::max(Vector3Length(centrePoint - flatMinPos), Vector3Length(centrePoint - flatMaxPos))); // *0.5f;

	

	/*float xWidth = std::abs((localSpaceBoundingBox.min.x) - (localSpaceBoundingBox.max.x));
	float zWidth = std::abs((localSpaceBoundingBox.min.z) - (localSpaceBoundingBox.max.z));
	float widthAverage = (xWidth + zWidth) * 0.5f;
	float radius = widthAverage * 0.5f;*/
	
	// m_collisionRadiusMax = m_hitbox.max.x
}

void StreetFurniture::setRelativePosition(Vector3 t_mapPos)
{
	m_position = t_mapPos + m_placementOffset;

	setHitBox(); // Update hitbox when position is set.

	m_stones[0].position = m_position + Vector3{ 2.0f, 0.0f, 0.0f };
	m_stones[1].position = m_position + Vector3{ 0.0f, 0.0f, 2.0f };
	m_stones[2].position = m_position + Vector3{ -2.0f, 0.0f, 0.0f };
	
	m_grassPos = m_position;

	if (m_hasFeeder)
	{
		Vector3 feedingPos = m_position;
		feedingPos.y = m_highestPoint; // m_hitbox.max.y;
		m_feeder.spawn(feedingPos);
	}
}

//bool StreetFurniture::checkPlayerFurnitureCollision(BoundingBox t_player)
//{
//	if (CheckCollisionBoxes(m_hitbox, t_player)){return true;}
//}

// Problems:
// * Clamp doesn't seem to work - collision circle always higher than I want it to be. // Spherical collision?
// * Still need to add exponential curve to collision shape 

//bool StreetFurniture::checkRadialFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRad)
//{
//	g_furnCollisionItem = m_position;	// Update debug values
//	g_furnCollisionPlyr = t_playerPos;	// Update debug values
//
//	m_posWithPlayerHeight = m_position;
//	m_posWithPlayerHeight.y = (t_playerPos.y) - m_position.y; // Player's current height minus lowest level of furniture
//	m_posWithPlayerHeight.y = Clamp(m_posWithPlayerHeight.y, m_position.y, m_overallHeightOnGround);
//
//	// m_posWithPlayerHeight.y -= 1.0f; // Stupid hack.
//
//	m_posWPlyrHeightNorm = m_posWithPlayerHeight;
//	m_posWPlyrHeightNorm.y = m_posWPlyrHeightNorm.y / m_overallHeightOnGround;
//
//	Vector3 playerTest = t_playerPos;
//	playerTest.y = 0.0f;
//
//	Vector3 mushTest = m_posWithPlayerHeight;
//	mushTest.y = 0.0f;
//
//	float distance = Vector3Distance(playerTest, mushTest);
//	// float distance = Vector3Distance(t_playerPos, m_posWithPlayerHeight);
//
//	// m_interpolatedColRadius = Lerp(m_collisionRadiusMin, m_collisionRadiusMax, m_posWPlyrHeightNorm.y);
//
//	m_interpolatedColRadius = exponentialScale(m_posWPlyrHeightNorm.y, m_collisionRadiusMin, m_collisionRadiusMax - 2.0f, 2.0f); // Note magic literal 2.0f =(
//
//	float combinedRad = t_playerRad + m_interpolatedColRadius;
//
//	// float yDelta = t_playerPos.y - m_overallHeightOnGround;
//
//	if (distance < combinedRad)	
//	{
//		if (t_playerPos.y < m_overallHeightOnGround)
//		{
//			g_lastFurnitureCollision = m_posWithPlayerHeight;
//			g_lastFurnitureRadius = m_interpolatedColRadius;
//			bool returnValue = true;
//			return returnValue;
//		}
//	}
//	return false;
//}

bool StreetFurniture::checkBoundsFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRadius, BoundingBox t_playerBox)
{
	if (!m_hasCollider) return false; // Early out - if we don't have colliders to test, what's the point?

	float xDist = t_playerPos.x - m_position.x;
	float zDist = t_playerPos.z - m_position.z;
	float combinedDist = xDist * xDist + zDist * zDist;
	float combinedRadius = t_playerRadius + FURNITURE_TEST_OUTER_RADIUS;
	float combinedRadResult = combinedRadius * combinedRadius;

	if (combinedDist > combinedRadResult) {
		//std::cout << "\nNo mushroom.\n";  
		return false; // RS: Early out - we're too far from the mushroom to bother checking anything else. Too mushroom! ...It's 6.47am.
	}

	//std::cout << "\nClose to a mushroom?\n";

	for (int i = 0; i < m_modelBoundingBoxes.size(); i++)
	//for (BoundingBox box : m_modelBoundingBoxes)
	{
		if (CheckCollisionBoxes(t_playerBox, m_modelBoundingBoxes[i])) { // box)) {

			switch (m_typeEnum)
			{
			case NONE:
				std::cout << "\nColliding with mushroom type: " << "NONE\n";
				break;
			case DEFAULT_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "DEFAULT_MUSHROOM\n"; "";
				break;
			case BATCH_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "BATCH_MUSHROOM\n"; "";
				break;
			case BUMPY_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "BUMPY_MUSHROOM\n"; "";
				break;
			case CHUNKY_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "CHUNKY_MUSHROOM\n"; "";
				break;
			case POINTY_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "POINTY_MUSHROOM\n"; "";
				break;
			case NOT_MUSHROOM:
				std::cout << "\nColliding with mushroom type: " << "NOT_MUSHROOM\n";
				break;
			default:
				break;
			}

			return true; // Collision detected!
		}
	}
	return false;
}



bool StreetFurniture::checkFeederBulletCollision(Vector3 t_bulletPos, float t_bulletRadius)
{
	if (m_hasFeeder)
	{
		if (CheckCollisionBoxSphere(m_feeder.getHitbox(), t_bulletPos, t_bulletRadius))
		{
			m_feeder.collision(true);
			return true;
		}
	}
	return false;
}

bool StreetFurniture::checkMudbombPlayerCollision(BoundingBox t_player)
{
	if (m_hasFeeder)
	{
		if (m_feeder.checkBulletCollisions(t_player))
		{
			return true;
		}
	}
	return false;
}

void StreetFurniture::makeFeederSeekPlayer(bool t_seeking, Player player)
{
	if (m_hasFeeder)
	{
		if (t_seeking)
		{
			m_feeder.makeActive();
		}
		else
		{
			m_feeder.disableShooting();
		}
	}
}

void StreetFurniture::makeFeederEat()
{
	if (m_hasFeeder && m_feeder.isAlive())
	{
		handleInput(EVENT_EAT);
	}
}

float StreetFurniture::exponentialScale(float scalar, float minimum, float maximum, float base)
{
	return minimum + (maximum - minimum) * (pow(base, scalar) - 1) / (base - 1);
}