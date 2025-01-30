#include "Enemy.h"

Enemy::Enemy() : m_position({2.0f, 2.0f, 0.0f})
{
	
}

void Enemy::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
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
