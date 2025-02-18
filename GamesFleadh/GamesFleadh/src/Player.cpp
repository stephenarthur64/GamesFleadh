#include "Player.h"


Player::Player() : m_speed(0.2f),  bulletCount(0)
{
	currentState = new IdleState;
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Player/Buzzz/Buzz.glb", &animsCount);
}

void Player::move(Vector3 t_velocity)
{
	t_velocity *= {m_speed, -m_speed, m_speed};
	m_position += t_velocity;
	m_hitbox.min += t_velocity;
	m_hitbox.max += t_velocity;
}

void Player::setHitBox()
{
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
		m_colour = WHITE;
	}
}

void Player::updateZPos(float newXPos)
{
	m_position.z = newXPos;
}

void Player::rotate(int t_direction)
{
	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
	
}

void Player::init()
{
	m_body = LoadModel("ASSETS/3D/Player/Buzzz/planet.glb");
	setHitBox();

	//m_weapon.init();
	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].init();
	}
}

void Player::render()
{
	DrawModel(m_body, m_position, 2.0f, m_colour);
	//DrawBoundingBox(m_hitbox, RED);

	//m_weapon.render();

	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].render();
	}
}

void Player::update()
{
	currentState->update(this);
	//m_weapon.update(m_position);
}

void Player::resetToOrigin()
{
	m_position.z = -9.0f;
	setHitBox();
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

