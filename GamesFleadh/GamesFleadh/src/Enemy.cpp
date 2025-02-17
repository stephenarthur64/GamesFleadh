#include "Enemy.h"

Enemy::Enemy()
{
	currentState = new IdleState;
	m_health = 1;
	m_position = { 2.0f, 2.0f, -10.0f };
	m_colour = RED;
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Enemy/Feeder/Feeder.glb", &animsCount);// <------------ Here for Animation of Feeder
	//modelAnimations = LoadModelAnimations("ASSETS/RS/animTest.glb", &animsCount);
}

void Enemy::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
	m_hitbox.min.z = m_position.z + hitboxOffsetMin;
	m_hitbox.max.z = m_position.z + hitboxOffsetMax;
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

void Enemy::spawn(Vector3 t_position)
{
	m_health = 10;

	m_position.x = t_position.x - 1.0f;
	m_position.y = t_position.y + 5.0f;
	m_position.z = t_position.z + 1.0f;

	m_hitbox.min.y = t_position.y + 5.0f;
	m_hitbox.max.y = t_position.y + 6.0f;

	m_hitbox.min.x = t_position.x - 1.5f;
	m_hitbox.max.x = t_position.x - 0.5f;

	m_hitbox.min.z = t_position.z + 0.5f;
	m_hitbox.max.z = t_position.z - 0.5f;
}

void Enemy::collision(bool t_collision)
{
	if (t_collision)
	{
		damageTick = 0;

		//m_colour = BLUE;
		m_health--;
		handleInput(Event::EVENT_DAMAGE);

		if (m_health <= 0)
		{
			kill();
		}
	}
	else
	{
		m_colour = WHITE;
	}
}

void Enemy::init()
{
	m_body = LoadModel("ASSETS/3D/Enemy/Feeder/Feeder.glb"); //<---------- Here for model change of Feeder
	m_yaw = 200.0f;
	//m_body = LoadModel("ASSETS/RS/animTest.glb");
	setHitBox();
	m_mudBomb.init();
}

void Enemy::render()
{
	DrawModel(m_body, m_position, 0.8f, m_colour);
	DrawBoundingBox(m_hitbox, GREEN);
	m_mudBomb.render();
}

void Enemy::shootBullet()
{
	bulletTick = 0;
	m_mudBomb.spawn(m_position, 0.3f);
}

void Enemy::despawnBullet()
{
	m_mudBomb.despawn();
}

void Enemy::disableShooting()
{
	bulletTick = -1;
}

void Enemy::kill()
{
	m_hitbox.min.x = 1000.0f;
	m_hitbox.max.x = 1001.0f;
	disableShooting();

	//m_position.x = 1000.0f;
}

void Enemy::update()
{
	currentState->update(this);
	m_mudBomb.move();

	if (bulletTick >= 180)
	{
		bulletTick = 0;
		shootBullet();
	}
	else if (bulletTick > -1)
	{
		bulletTick++;
	}

	if (damageTick >= 60)
	{
		if (m_health > 0)
		{
			handleInput(Event::EVENT_NONE);
		}
		else
		{
			handleInput(Event::EVENT_DIE);
		}
	}
	else if (damageTick > -1)
	{
		damageTick++;
	}
}

void Enemy::rotate(int t_direction)
{
}
