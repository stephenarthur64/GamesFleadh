#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::spawn(Vector3 t_pos, float t_speed)
{
	m_position = t_pos;
	m_speed = t_speed;
	m_active = true;
}

void Bullet::move()
{
	if (m_active)
	{
		m_position.x += m_speed;
	}
}
