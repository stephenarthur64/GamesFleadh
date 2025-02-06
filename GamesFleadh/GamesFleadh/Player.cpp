#include "Player.h"


Player::Player() : m_speed(0.3f), m_position({7.0f, 0.0f, 0.0f}), bulletCount(0), m_colour(GREEN), m_roll(0.0f), m_pitch(0.0f)
{
}

void Player::move(int t_directon)
{
	if (t_directon == NORTH)
	{
		m_pitch += 1.0f;
		m_position.y += m_speed;
		m_hitbox.min.y += m_speed;
		m_hitbox.max.y += m_speed;
	}
	if (t_directon == SOUTH)
	{
		m_pitch -= 1.0f;
		m_position.y -= m_speed;
		m_hitbox.min.y -= m_speed;
		m_hitbox.max.y -= m_speed;
	}
	if (t_directon == EAST)
	{
		m_roll += 1.0f;
		m_position.x += m_speed;
		m_hitbox.min.x += m_speed;
		m_hitbox.max.x += m_speed;
	}
	if (t_directon == WEST)
	{
		m_roll -= 1.0f;
		m_position.x -= m_speed;
		m_hitbox.min.x -= m_speed;
		m_hitbox.max.x -= m_speed;
	}
	updateModelRotate();
	//updateHitBox();
}

void Player::setHitBox()
{
	m_hitbox = GetMeshBoundingBox(m_body.meshes[0]);
	m_hitbox.min.x = m_position.x - hitboxOffsetMin;
	m_hitbox.max.x = m_position.x - hitboxOffsetMax;

	m_hitbox.min.y = m_position.y - 0.4f;
	m_hitbox.max.y = m_position.y + 0.6f;

	m_hitbox.min.z = m_position.y - 6.0f;
	m_hitbox.max.z = m_position.y - 7.5f;
}

void Player::updateHitBox(float t_z)
{
	m_hitbox.min.z += t_z;
	m_hitbox.max.z += t_z;
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

void Player::updateZPos(float newXPos)
{
	m_position.z = newXPos;
}

void Player::updateModelRotate()
{
	//m_body.transform = MatrixRotateXYZ({ 0, DEG2RAD * m_pitch, DEG2RAD* m_roll });
}

void Player::resetToOrigin()
{
	m_position.x = 0.0f;
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
