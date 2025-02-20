#include "Swarmer.h"

Swarmer::Swarmer() : m_speed(0.1f), m_direction(NORTH), m_spotted(false)
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
}

void Swarmer::render()
{
	DrawModel(m_body, m_position, 1.0f, WHITE);
}

void Swarmer::setLimits(float t_upperLimit, float t_lowerLimit)
{
	m_upperLimit = t_upperLimit;
	m_lowerLimit = t_lowerLimit;
}

void Swarmer::update()
{
	currentState->update(this);
	
	playerSpotted(m_spotted);

	if (spottedTick >= 48)
	{
		//m_spotted = false;
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

		if (m_upperLimit < m_position.y)
		{
			m_direction = SOUTH;
		}
	}

	if (m_direction == SOUTH)
	{
		m_position.y -= m_speed;

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

	if (distance <= 2.0f)
	{
		m_spotted = true;
	}
	else
	{
		m_spotted = false;
	}
}
