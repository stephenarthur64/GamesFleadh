#include "Feeder.h"

Feeder::Feeder()
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
{
	m_health = 1;

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

void Feeder::collision(bool t_collision)
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

void Feeder::init()
{
	m_body = LoadModel("ASSETS/3D/Enemy/Feeder/Feeder.glb"); //<---------- Here for model change of Feeder
	m_yaw = 200.0f;
	//m_body = LoadModel("ASSETS/RS/animTest.glb");
	setHitBox();
	m_mudBomb.init();
	fxBoom = LoadSound("ASSETS/boom.wav");
	SetSoundVolume(fxBoom, 0.3);
	explosion = LoadTexture("ASSETS/explosion.png");
	frameWidth = (float)(explosion.width / NUM_FRAMES_PER_LINE);   // Sprite one frame rectangle width
	frameHeight = (float)(explosion.height / NUM_LINES);           // Sprite one frame rectangle height
	frameRec = { 0, 0, frameWidth, frameHeight };
}

void Feeder::render()
{
	DrawModel(m_body, m_position, 0.8f, m_colour);
	DrawBoundingBox(m_hitbox, GREEN);
	m_mudBomb.render();
}

void Feeder::renderBoom(Camera &t_camera)
{
	if (active)
	{
		DrawBillboard(t_camera, explosion, m_position, 2.0f, WHITE);
		//DrawTextureRec(explosion, frameRec, position, WHITE);
	}
}

void Feeder::shootBullet(Vector3 t_target)
{
	m_target = t_target;
	bulletTick = 0;
	m_mudBomb.spawn(m_position, 0.3f, m_target);
}

void Feeder::despawnBullet()
{
	m_mudBomb.despawn();
}

void Feeder::disableShooting()
{
	bulletTick = -1;
}

void Feeder::boom()
{
	if (active)
	{
		framesCounter++;

		if (framesCounter > 2)
		{
			currentFrame++;

			if (currentFrame >= NUM_FRAMES_PER_LINE)
			{
				currentFrame = 0;
				currentLine++;

				if (currentLine >= NUM_LINES)
				{
					currentLine = 0;
					active = false;
				}
			}

			framesCounter = 0;
		}
	}
}

void Feeder::kill()
{
	m_hitbox.min.x = 1000.0f;
	m_hitbox.max.x = 1001.0f;
	disableShooting();
	active = true;

	position.x = m_position.x;
	position.y = m_position.y;

	PlaySound(fxBoom);

	//m_position.x = 1000.0f;
}

void Feeder::update(Vector3 t_target)
{
	currentState->update(this);
	m_mudBomb.follow();

	boom();

	if (bulletTick >= 180)
	{
		bulletTick = 0;
		shootBullet(t_target);
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

void Feeder::rotate(int t_direction)
{
}
