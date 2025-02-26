#include "StreetFurniture.h"
#include <iostream>

StreetFurniture::StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos, FurnitureType t_type) : m_hasFeeder(t_hasFeeder),
																															m_colourDecrease(0),
																															m_colourVal(255), eatTick(0), 
																															MAX_EAT_TICK(60),
																															m_type(t_type)
																									
{
	m_placementOffset = t_startPos;
	currentState = new IdleState;
	m_body = LoadModel(t_furnitureType.c_str());
	/*for (int i = 0; i < m_body.materialCount; i++)
	{
		m_body.materials[i].shader = LoadShader(TextFormat("ASSETS/shaders/glsl%i/discard_alpha.fs", GLSL_VERSION),
			TextFormat("ASSETS/shaders/glsl%i/discard_alpha.fs", GLSL_VERSION));
	}*/
	// setHitBox();	
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations(t_furnitureType.c_str(), &animsCount);
	if (t_hasFeeder)
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

void StreetFurniture::rotate(int t_direction){}

void StreetFurniture::init()
{
}

void StreetFurniture::render()
{
	if (!m_inPlay) return; // Not in gameplay: early out.

	DrawModel(m_body, m_position, 1.0f, m_colour);
	DrawBoundingBox(m_hitbox, BLUE);

	for (int i = 0; i < 3; i++)
	{
		DrawModel(m_stones[i].body, m_stones[i].position, 0.5f, WHITE);
	}
	DrawModel(m_grass, m_grassPos, 1.0f, WHITE);
	if (!m_hasFeeder) return;
	m_feeder.render();
}

void StreetFurniture::initStones()
{
	if (m_type == CHUNKY_MUSHROOM) // Big mushroom needs large stones
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
	if (m_type == MUSHROOM || m_type == CHUNKY_MUSHROOM)
	{
		currentState->update(this);
	}

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
		feedingPos.y = m_hitbox.max.y;
		m_feeder.spawn(feedingPos);
	}
}

bool StreetFurniture::checkPlayerFurnitureCollision(BoundingBox t_player)
{
	if (CheckCollisionBoxes(m_hitbox, t_player)){return true;}
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
	if (m_hasFeeder)
	{
		handleInput(EVENT_DAMAGE);
	}
}
