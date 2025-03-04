#include "Bullet.h"

Bullet::Bullet() : m_radius(0.4f)
{
}

void Bullet::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
}

void Bullet::spawn(Vector3 t_pos, float t_speed, Vector3 t_velocity)
{
	setHitBox();
	m_position = t_pos;
	m_speed = t_speed;
	m_active = true;
	m_target = t_velocity;
	distance = 0;
	//m_velocity *= {m_speed, m_speed, m_speed};

	m_velocity = {
		m_target.x - m_position.x,
		m_target.y - m_position.y,
		m_target.z - m_position.z
	};

	// Heere come de RoB code:
	m_direction = t_velocity - t_pos;
	m_direction = Vector3Normalize(m_direction);

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
		/*distance += 2.0f;
		m_target += {0.5f, 0.5f, -2.0f};


		m_position = Vector3MoveTowards(m_position, m_target, distance);*/
		
		/*Vector3 velocity = Vector3Normalize(m_target) * m_speed;

		m_position += velocity;*/

		/*m_position += m_direction * m_speed;*/

		// Calculate the length (magnitude) of the direction vector
		float length = sqrtf(m_velocity.x * m_velocity.x +
			m_velocity.y * m_velocity.y +
			m_velocity.z * m_velocity.z);

		// Normalize the direction vector
		if (length > 0.0001f)
		{
			m_velocity.x /= length;
			m_velocity.y /= length;
			m_velocity.z /= length;
		}

		// Move projectile
		m_position.x += m_velocity.x * m_speed;
		m_position.y += m_velocity.y * m_speed;
		m_position.z += m_velocity.z * m_speed;
	}
}

void Bullet::directMove()
{
	if (m_active)
	{
		m_position += m_direction * m_speed;
	}
}

void Bullet::follow(Vector3 t_target)
{
	if (m_active)
	{
		distance += 0.001f;
		m_position = Vector3MoveTowards(m_position, t_target, distance);
	}
}

void Bullet::init(int t_type)
{
	if (t_type == 0)
	{
		m_body = LoadModel("ASSETS/3D/Player/Bullet/Bullet.glb");
	}
	else
	{
		m_body = LoadModel("ASSETS/3D/Enemy/Feeder/enemyBullet.glb");
	}
}

void Bullet::render()
{
	DrawModel(m_body, m_position, m_radius, WHITE);
	DrawBoundingBox(m_hitbox, RED);
}

void Bullet::rotate(int t_direction)
{
}
