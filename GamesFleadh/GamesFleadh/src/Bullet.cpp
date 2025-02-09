#include "Bullet.h"

Bullet::Bullet()
{
}

void Bullet::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
}

void Bullet::spawn(Vector3 t_pos, float t_speed)
{
	setHitBox();
	m_position = t_pos;
	m_speed = t_speed;
	m_active = true;
}

void Bullet::despawn()
{
	m_active = false;
	m_position.y = 10000.0f;
}

void Bullet::move()
{
	if (m_active)
	{
		m_position.x += m_speed;
	}
}
