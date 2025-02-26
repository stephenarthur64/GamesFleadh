#include "StreetFurniture.h"
#include <iostream>

StreetFurniture::StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos) : m_hasFeeder(t_hasFeeder)
{
	m_placementOffset = t_startPos;

	m_body = LoadModel(t_furnitureType.c_str());
	// setHitBox();	

	if (t_hasFeeder)
	{
		m_feeder.init();
	}
}

StreetFurniture::~StreetFurniture()
{
	/*if (m_feeder == nullptr) return;
	delete m_feeder;*/
}

void StreetFurniture::rotate(int t_direction){}

void StreetFurniture::init(){}

void StreetFurniture::render()
{
	if (!m_inPlay) return; // Not in gameplay: early out.

	DrawModel(m_body, m_position, 1.0f, WHITE);
	DrawBoundingBox(m_hitbox, BLUE);
	
	// DrawCylinderWires(m_position, m_collisionRadiusMin, m_collisionRadiusMin, 100.0f, 6, GREEN);

	DrawCircle3D(m_posWithPlayerHeight, m_interpolatedColRadius, Vector3{ 1.0f, 0.0f, 0.0f }, 90.0f, ORANGE);



	if (!m_hasFeeder) return;
	m_feeder.render();
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
	if (!m_hasFeeder) return;
	m_feeder.update(t_target);
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

	if (m_hasFeeder)
	{
		Vector3 feedingPos = m_position;
		feedingPos.y = m_hitbox.max.y;
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

bool StreetFurniture::checkRadialFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRad)
{
	m_posWithPlayerHeight = m_position;
	m_posWithPlayerHeight.y = (t_playerPos.y) - m_position.y; // Player's current height minus lowest level of furniture
	m_posWithPlayerHeight.y = Clamp(m_posWithPlayerHeight.y, m_position.y, m_overallHeightOnGround);

	// m_posWithPlayerHeight.y -= 1.0f; // Stupid hack.

	m_posWPlyrHeightNorm = m_posWithPlayerHeight;
	m_posWPlyrHeightNorm.y = m_posWPlyrHeightNorm.y / m_overallHeightOnGround;

	Vector3 playerTest = t_playerPos;
	playerTest.y = 0.0f;

	Vector3 mushTest = m_posWithPlayerHeight;
	mushTest.y = 0.0f;

	float distance = Vector3Distance(playerTest, mushTest);
	// float distance = Vector3Distance(t_playerPos, m_posWithPlayerHeight);

	m_interpolatedColRadius = Lerp(m_collisionRadiusMin, m_collisionRadiusMax, m_posWPlyrHeightNorm.y);

	float combinedRad = t_playerRad + m_interpolatedColRadius;

	if (distance < combinedRad)	
	{
		bool returnValue = true;
		return returnValue; 
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
