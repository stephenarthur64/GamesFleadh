#include "Player.h"


Player::Player() : m_speed(0.3f), m_position({8.0f, 0.0f, 0.0f}), bulletCount(0), m_colour(GREEN)
{
}

void Player::move(int t_directon)
{
	if (t_directon == NORTH)
	{
		m_position.y += m_speed;
		m_hitbox.min.y += m_speed;
		m_hitbox.max.y += m_speed;
	}
	if (t_directon == SOUTH)
	{
		m_position.y -= m_speed;
		m_hitbox.min.y -= m_speed;
		m_hitbox.max.y -= m_speed;
	}
	if (t_directon == EAST)
	{
		m_position.z += m_speed;
		m_hitbox.min.z += m_speed;
		m_hitbox.max.z += m_speed;
	}
	if (t_directon == WEST)
	{
		m_position.z -= m_speed;
		m_hitbox.min.z -= m_speed;
		m_hitbox.max.z -= m_speed;
	}
	//updateHitBox();
}

void Player::setHitBox()
{
	m_hitbox = GetMeshBoundingBox(m_body.meshes[0]);
	m_hitbox.min.x = m_position.x - camOffsetMin;
	m_hitbox.max.x = m_position.x - camOffsetMax;
}

void Player::updateHitBox(float t_x)
{
	m_hitbox.min.x += t_x;
	m_hitbox.max.x += t_x;
}

void Player::collision(bool collide)
{
	if (collide)
	{
		m_colour = RED;
	}
	else
	{
		m_colour = GREEN;
	}
}

void Player::shootBullet()
{
	if (bulletCount < 10)
	{
		bullet[bulletCount].spawn(m_position, -0.3f);
		bulletCount++;
	}
	else
	{
		bulletCount = 0;
		bullet[bulletCount].spawn(m_position, -0.3f);
	}
}

void Player::updateBullet()
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullet[i].move();
	}
}

void Player::despawnBullet(int bulletNum)
{
	bullet[bulletNum].despawn();
}
