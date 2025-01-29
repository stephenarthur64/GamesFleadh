#include "Player.h"


Player::Player() : m_speed(0.3f), m_position({0.0f, 0.0f, 0.0f})
{
}

void Player::move(int t_directon)
{
	if (t_directon == NORTH)
	{
		m_position.y += m_speed;
	}
	if (t_directon == SOUTH)
	{
		m_position.y -= m_speed;
	}
	if (t_directon == EAST)
	{
		m_position.z += m_speed;
	}
	if (t_directon == WEST)
	{
		m_position.z -= m_speed;
	}
}
