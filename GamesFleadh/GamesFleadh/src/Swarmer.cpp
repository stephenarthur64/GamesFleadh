#include "Swarmer.h"

Swarmer::Swarmer() : m_speed(0.1f), m_direction(NORTH), m_spotted(false), MAX_DISTANCE(6.0f)
{
	currentState = new IdleState;
	m_position = { -2.0f, 1.0f, -12.0f };
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Enemy/Swarmer/Swarmer.glb", &animsCount);
}

void Swarmer::rotate(int t_direction)
{
}

void Swarmer::init()
{
	m_body = LoadModel("ASSETS/3D/Enemy/Swarmer/Swarmer.glb");
	setLimits(5, 0);
	setHitbox();
}

void Swarmer::render()
{
	DrawModel(m_body, m_position, 1.0f, WHITE);
	DrawBoundingBox(m_hitbox, BLUE);
}

void Swarmer::setLimits(float t_upperLimit, float t_lowerLimit)
{
	m_upperLimit = t_upperLimit;
	m_lowerLimit = t_lowerLimit;
}

void Swarmer::setHitbox()
{
	Vector3 minOffset = { -1.0f, -0.5f, -1.0f };
	Vector3 maxOffset = { 1.0f, 1.5f, 1.0f };

	m_hitbox.min = m_position + minOffset;
	m_hitbox.max = m_position + maxOffset;

}

void Swarmer::update()
{
	currentState->update(this);
	
	playerSpotted(m_spotted);

	if (spottedTick >= 48)
	{
		handleInput(Event::EVENT_CHASE);
		chasePlayer();
	}

	if (!m_spotted)
	{
		hover();
	}

}

void Swarmer::playerSpotted(bool t_spotted)
{
	if (t_spotted)
	{
		spottedTick++;
		handleInput(Event::EVENT_SPOTTED);
	}
	else
	{
		spottedTick = 0;
		handleInput(Event::EVENT_NONE);
	}
}

void Swarmer::hover()
{
	if (m_direction == NORTH)
	{
		m_position.y += m_speed;
		m_hitbox.min.y += m_speed;
		m_hitbox.max.y += m_speed;

		if (m_upperLimit < m_position.y)
		{
			m_direction = SOUTH;
		}
	}

	if (m_direction == SOUTH)
	{
		m_position.y -= m_speed;
		m_hitbox.min.y -= m_speed;
		m_hitbox.max.y -= m_speed;

		if (m_lowerLimit > m_position.y)
		{
			m_direction = NORTH;
		}
	}
}

void Swarmer::checkDistanceFromPlayer(Vector3 t_playerPos)
{
	float xDistance = (t_playerPos.x - m_position.x) * (t_playerPos.x - m_position.x);
	float zDistance = (t_playerPos.z - m_position.z) * (t_playerPos.z - m_position.z);

	float distance = sqrtf(xDistance + zDistance);

	if (distance <= MAX_DISTANCE)
	{
		m_target = t_playerPos;
		m_spotted = true;
	}
	else
	{
		m_spotted = false;
	}
}

void Swarmer::chasePlayer()
{
	m_velocity = m_target - m_position;
	m_velocity = Vector3Normalize(m_velocity);
	m_velocity *= m_speed;

	m_position += m_velocity;
	m_hitbox.min += m_velocity;
	m_hitbox.max += m_velocity;
}
