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
	
	DrawCylinderWires(m_position, m_collisionRadiusMin, m_collisionRadiusMin, 100.0f, 6, GREEN);

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

	m_overallHeightOnGround = m_hitbox.max.y - m_hitbox.min.y;
	m_overallHeightOnGround += m_position.y;
	
	//m_collisionRadiusMax = m_hitbox.max.x
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

bool StreetFurniture::checkRadialFurnitureItemsCollision(Vector3 t_playerPos, float t_playerRad)
{// This file not saving is very weird.
	Vector3 posWithPlayerHeight = m_position;
	posWithPlayerHeight.y = t_playerPos.y;

	float distance = Vector3Distance(t_playerPos, posWithPlayerHeight);

	float combinedRad = t_playerRad + m_collisionRadiusMin;

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
