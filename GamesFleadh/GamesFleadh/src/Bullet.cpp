#include "Bullet.h"

Bullet::Bullet()
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
	m_velocity = t_velocity;
	distance = 0;
	//m_velocity *= {m_speed, m_speed, m_speed};

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
		/*distance += 0.5f;
		m_velocity.z -= 0.5f;


		m_position = Vector3MoveTowards(m_position, m_velocity, distance);*/
		
		//m_position += m_velocity;

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

void Bullet::init()
{
	m_body = LoadModel("ASSETS/RS/bulletProto.glb");
}

void Bullet::render()
{
	DrawModel(m_body, m_position, 0.5f, BLUE);
	DrawBoundingBox(m_hitbox, RED);
}

void Bullet::rotate(int t_direction)
{
}
