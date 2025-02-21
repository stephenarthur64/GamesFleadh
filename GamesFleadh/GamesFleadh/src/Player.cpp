#include "Player.h"


Player::Player() : m_speed(0.2f),  bulletCount(0), HEALTHBAR_MAX(450)
{
	currentState = new IdleState;
	m_health = 100;
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

	m_hitbox.min.y = m_position.y - 0.1f;
	m_hitbox.max.y = m_position.y + 1.1f;

	m_hitbox.min.z = m_position.z + 0.5f;
	m_hitbox.max.z = m_position.z - 1.0f;
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
		handleInput(Event::EVENT_DAMAGE);
		m_health--;
	}
}

void Player::worldCollision(bool collide)
{
	if (collide)
	{
		handleInput(Event::EVENT_DAMAGE);
	}
	else
	{
		m_colour = WHITE;
	}
}

void Player::updateZPos(float newXPos)
{
	m_position.z = newXPos; 
	setHitBox();
}

void Player::rotate(int t_direction)
{
	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
	
}

void Player::init()
{
	m_body = LoadModel("ASSETS/3D/Player/Buzzz/Buzz.glb");
	setHitBox();

	m_healthbar.x = 40;
	m_healthbar.y = 1015;
	m_healthbar.width = HEALTHBAR_MAX;
	m_healthbar.height = 40;

	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].init();
	}
}

void Player::render()
{
	DrawModel(m_body, m_position, 2.0f, m_colour);
	DrawBoundingBox(m_hitbox, RED);

	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].render();
	}
}

void Player::update()
{
	currentState->update(this);
	updateHealthbar();
	//m_weapon.update(m_position);
}

void Player::updateHealthbar()
{
	float onePercent = HEALTHBAR_MAX / 100;
	m_healthbar.width = m_health * onePercent;
}

void Player::resetToOrigin()
{
	std::cout << "\nResetting player's position.";
	m_position.z = 16.0f;
	setHitBox();
}

void lookat_angles(Vector3* target, float* yaw, float* pitch) 
{
	*yaw = -(atan2(target->x, target->y) * RAD2DEG) + 90.0f;
	float distance = sqrt(target->x * target->x + target->z * target->z);
	*pitch = -(-atan2(target->y, distance) * RAD2DEG);
}

void Player::faceCrosshair(Vector3 t_crosshairPos)
{
	Matrix mat = MatrixLookAt(m_position, t_crosshairPos, { 0,1,0 });

	mat = MatrixInvert(mat);

	Vector3 translation = { 0 };
	Quaternion crosshairRotation = { 0 };
	Vector3 scale = { 0 };
	
	MatrixDecompose(mat, &translation, &crosshairRotation, &scale);


	m_body.transform = MatrixMultiply(MatrixRotateY(DEG2RAD * 90.0f), QuaternionToMatrix(crosshairRotation));
}

void Player::shootBullet(Vector3 t_target)
{
	if (bulletCount < 10)
	{
		bullet[bulletCount].spawn(m_position, 0.3f, t_target);
		bulletCount++;
	}
	else
	{
		bulletCount = 0;
		bullet[bulletCount].spawn(m_position, 0.3f, t_target);
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

