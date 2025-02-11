#include "Enemy.h"

Enemy::Enemy()
{
	m_position = { 2.0f, 2.0f, 3.0f };
	m_colour = RED;
}

void Enemy::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
	m_hitbox.min.z = m_position.z - hitboxOffsetMin;
	m_hitbox.max.z = m_position.z - hitboxOffsetMax;
	updateHitBox();
}

void Enemy::updateHitBox()
{
	m_hitbox.min.x += m_position.x;
	m_hitbox.min.y += m_position.y;
	m_hitbox.min.z += m_position.z;

	m_hitbox.max.x += m_position.x;
	m_hitbox.max.y += m_position.y;
	m_hitbox.max.z += m_position.z;
}

void Enemy::collision(bool t_collision)
{
	if (t_collision)
	{
		m_colour = BLUE;
	}
	else
	{
		m_colour = RED;
	}
}

void Enemy::rotate(int t_direction)
{
}
