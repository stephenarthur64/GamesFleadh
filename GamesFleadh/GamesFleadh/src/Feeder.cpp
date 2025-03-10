#include "Feeder.h"

Feeder::Feeder() : MAX_DISTANCE(8.0f), m_spotted(false), m_active(false), BULLET_TICK_MAX(120), DAMAGE_TICK_MAX(60), boomColor(WHITE)
{
	currentState = new IdleState;
	m_health = 1;
	m_position = { 2.0f, 2.0f, -10.0f }; // Position is set from Tile (TileIsCurrent) -> StreetFurniture (SetRelativePosition) -> Feeder (Spawn)
	m_colour = WHITE;
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Enemy/Feeder/Feeder.glb", &animsCount);// <------------ Here for Animation of Feeder
	boomColor.a = 255;
	//modelAnimations = LoadModelAnimations("ASSETS/RS/animTest.glb", &animsCount);
}

void Feeder::setHitBox()
{
	m_hitbox = GetModelBoundingBox(m_body);
	m_hitbox.min.z = m_position.z + hitboxOffsetMin;
	m_hitbox.max.z = m_position.z + hitboxOffsetMax;
	updateHitBox();
}

void Feeder::updateHitBox()
{
	m_hitbox.min.x += m_position.x;
	m_hitbox.min.y += m_position.y;
	m_hitbox.min.z += m_position.z;

	m_hitbox.max.x += m_position.x;
	m_hitbox.max.y += m_position.y;
	m_hitbox.max.z += m_position.z;
}

void Feeder::spawn(Vector3 t_position)
{// Position is set from Tile (TileIsCurrent) -> StreetFurniture (SetRelativePosition) -> Feeder (Spawn)
	m_health = 1;

	currentState = new IdleState;

	m_position.x = t_position.x - 1.0f;
	m_position.y = t_position.y + 0.0f;
	m_position.z = t_position.z + 1.0f;

	m_hitbox.min.y = t_position.y + 0.0f;
	m_hitbox.max.y = t_position.y + 1.0f;

	m_hitbox.min.x = t_position.x - 1.5f;
	m_hitbox.max.x = t_position.x - 0.5f;

	m_hitbox.min.z = t_position.z + 0.5f;
	m_hitbox.max.z = t_position.z - 0.5f;
	despawnBullet(); 
	disableShooting();
	m_colour = WHITE;
}

void Feeder::collision(bool t_collision)
{
	if (t_collision)
	{
		PlaySound(sfxHit);
		damageTick = 0;

		m_health--;
		handleInput(Event::EVENT_DAMAGE);

		if (m_health == 0)
		{
			kill();
		}
	}
	else
	{
		m_colour = WHITE;
	}
}

void Feeder::init()
{
	m_body = LoadModel("ASSETS/3D/Enemy/Feeder/Feeder.glb"); //<---------- Here for model change of Feeder
	m_yaw = 200.0f;
	//m_body = LoadModel("ASSETS/RS/animTest.glb");
	setHitBox();
	m_mudBomb.init(1);
	sfxDeath = LoadSound("ASSETS/Audio/SFX/Feeder/feederDeath.mp3");
	sfxHit = LoadSound("ASSETS/Audio/SFX/Feeder/feederHit.mp3");
	sfxMissileLaunch = LoadSound("ASSETS/Audio/SFX/Feeder/feederMissileLaunchRedux.mp3");
	// ------------------------------------------------- Not Implemented Yet
	sfxFeeding = LoadSound("ASSETS/Audio/SFX/Feeder/feederFeeding.mp3");
	sfxMissileHit = LoadSound("ASSETS/Audio/SFX/Feeder/feederMissileHitRedux.mp3");
	SetSoundVolume(sfxDeath, 0.3);
	SetSoundVolume(sfxHit, 0.3);
	SetSoundVolume(sfxMissileLaunch, 0.05);
	explosion = LoadTexture("ASSETS/2D/explosion.png");
	frameWidth = (float)(explosion.width / NUM_FRAMES_PER_LINE);   // Sprite one frame rectangle width
	frameHeight = (float)(explosion.height / NUM_LINES);           // Sprite one frame rectangle height
	frameRec = { 0, 0, frameWidth, frameHeight };
}

void Feeder::render()
{
	if (g_renderWireDebug)
	{
		DrawBoundingBox(m_hitbox, GREEN);
	}

	DrawModel(m_body, m_position, 1.8f, m_colour);	
	m_mudBomb.render();
}

void Feeder::renderBoom(Camera &t_camera)
{
	if (boomActive)
	{
		DrawBillboard(t_camera, explosion, m_position, boomScale, boomColor);
		//DrawTextureRec(explosion, frameRec, position, WHITE);
	}
}

bool Feeder::checkBulletCollisions(BoundingBox t_player)
{
	if (CheckCollisionBoxSphere(t_player, m_mudBomb.getPosition(), m_mudBomb.getRadius()))
	{
		mudBombPosition = m_mudBomb.getPosition().x; // DON'T LOOK AT ME
		m_mudBomb.despawn();
		return true;
	}

	return false;
}

void Feeder::shootBullet(Vector3 t_target)
{
	checkDistanceFromPlayer(t_target);

	if (m_spotted)
	{
		PlaySound(sfxMissileLaunch);
		handleInput(EVENT_ATTACK);
		m_target = t_target;
		bulletTick = 0;
		m_mudBomb.spawn(m_position, 0.3f, m_target);
	}
}

void Feeder::despawnBullet()
{
	m_mudBomb.despawn();
	m_active = false;
}

void Feeder::disableShooting()
{
	bulletTick = -1;
}

void Feeder::boom()
{
	if (boomActive)
	{
		int alpha;
		int direction = -1;

		if (boomActive)
		{
			alpha = boomColor.a + (direction * 8);

			if (alpha > 0)
			{
				boomColor.a = alpha;
				boomScale += 0.2f;
			}
			else
			{
				boomActive = false;
			}
		}
	}
	else
	{
		boomColor.a = 255;
		boomScale = 0.0f;
	}
}

void Feeder::kill()
{
	m_hitbox.min.x = 1000.0f;
	m_hitbox.max.x = 1001.0f;
	disableShooting();
	boomActive = true;

	position.x = m_position.x;
	position.y = m_position.y;

	m_health = -1;

	PlaySound(sfxDeath);

	//m_position.x = 1000.0f;
}

bool Feeder::isAlive()
{
	if (m_health <= 0 && !boomActive)
	{
		return false;
	}

	return true;
}

void Feeder::update(Vector3 t_target)
{
	currentState->update(this);
	m_mudBomb.directMove();
	if (m_active && bulletTick < 0)
	{
		shootBullet(t_target);
	}

	boom();

	if (bulletTick >= BULLET_TICK_MAX && t_target.z > m_position.z)
	{
		m_mudBomb.despawn();
		disableShooting();
	}

	if (bulletTick >= BULLET_TICK_MAX)
	{
		bulletTick = 0;
		shootBullet(t_target);
	}
	else if (bulletTick > -1)
	{
		bulletTick++;
	}

	if (damageTick >= DAMAGE_TICK_MAX)
	{
		if (m_health > 0)
		{
			handleInput(Event::EVENT_MOVE);
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

void Feeder::checkDistanceFromPlayer(Vector3 t_playerPos)
{
	if (!m_spotted)
	{
		m_target = t_playerPos;
	}

	float xDistance = (m_target.x - m_position.x) * (m_target.x - m_position.x);
	float zDistance = (m_target.z - m_position.z) * (m_target.z - m_position.z);

	float distance = sqrtf(xDistance + zDistance);

	if (distance <= MAX_DISTANCE)
	{
		m_spotted = true;
	}
	else
	{
		m_spotted = false;
	}

	if (distance <= 0.5f)
	{
		m_spotted = false;
	}

	if (t_playerPos.z < m_position.z)
	{
		m_spotted = false;
	}
}

void Feeder::rotate(int t_direction)
{
}
