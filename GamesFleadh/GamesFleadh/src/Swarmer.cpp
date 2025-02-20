#include "Swarmer.h"

Swarmer::Swarmer() : m_speed(0.1f), m_direction(NORTH), m_spotted(false), MAX_DISTANCE(6.0f)
{
	currentState = new IdleState;
	m_health = 1;
	m_position = { -2.0f, 1.0f, -12.0f };
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Enemy/Swarmer/Swarmer.glb", &animsCount);
}

void Swarmer::rotate(int t_direction)
{
}

void Swarmer::init()
{
	m_body = LoadModel("ASSETS/3D/Enemy/Swarmer/Swarmer.glb");
	setLimits(5, 0);
	setHitbox();
	fxBoom = LoadSound("ASSETS/boom.wav");
	SetSoundVolume(fxBoom, 0.3);
	explosion = LoadTexture("ASSETS/explosion.png");
	frameWidth = (float)(explosion.width / NUM_FRAMES_PER_LINE);   // Sprite one frame rectangle width
	frameHeight = (float)(explosion.height / NUM_LINES);           // Sprite one frame rectangle height
	frameRec = { 0, 0, frameWidth, frameHeight };
}

void Swarmer::render()
{
	DrawModel(m_body, m_position, 1.0f, WHITE);
	DrawBoundingBox(m_hitbox, BLUE);
}

void Swarmer::renderBoom(Camera t_camera)
{
	if (active)
	{
		DrawBillboard(t_camera, explosion, m_position, 2.0f, WHITE);
	}
}

void Swarmer::setLimits(float t_upperLimit, float t_lowerLimit)
{
	m_upperLimit = t_upperLimit;
	m_lowerLimit = t_lowerLimit;
}

void Swarmer::setHitbox()
{
	Vector3 minOffset = { -1.0f, -0.5f, -1.0f };
	Vector3 maxOffset = { 1.0f, 1.5f, 1.0f };

	m_hitbox.min = m_position + minOffset;
	m_hitbox.max = m_position + maxOffset;

}

void Swarmer::collision(bool t_collide)
{
	if (t_collide)
	{
		m_health--;
		if (m_health <= 0)
		{
			kill();
		}
	}
}

void Swarmer::kill()
{
	m_hitbox.min.x = 1000.0f;
	m_hitbox.max.x = 1001.0f;
	active = true;

	PlaySound(fxBoom);
}

void Swarmer::boom()
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

void Swarmer::update()
{
	currentState->update(this);
	
	boom();

	playerSpotted(m_spotted);

	if (spottedTick >= 48)
	{
		handleInput(Event::EVENT_CHASE);
		chasePlayer();
	}

	if (!m_spotted && m_health > 0)
	{
		hover();
	}

}

void Swarmer::playerSpotted(bool t_spotted)
{
	if (t_spotted)
	{
		spottedTick++;
		handleInput(Event::EVENT_SPOTTED);
	}
	else
	{
		spottedTick = 0;
		handleInput(Event::EVENT_NONE);
	}
}

void Swarmer::hover()
{
	if (m_direction == NORTH)
	{
		m_position.y += m_speed;
		m_hitbox.min.y += m_speed;
		m_hitbox.max.y += m_speed;

		if (m_upperLimit < m_position.y)
		{
			m_direction = SOUTH;
		}
	}

	if (m_direction == SOUTH)
	{
		m_position.y -= m_speed;
		m_hitbox.min.y -= m_speed;
		m_hitbox.max.y -= m_speed;

		if (m_lowerLimit > m_position.y)
		{
			m_direction = NORTH;
		}
	}
}

void Swarmer::checkDistanceFromPlayer(Vector3 t_playerPos)
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
}

void Swarmer::chasePlayer()
{
	m_velocity = m_target - m_position;
	m_velocity = Vector3Normalize(m_velocity);
	m_velocity *= m_speed;

	m_position += m_velocity;
	m_hitbox.min += m_velocity;
	m_hitbox.max += m_velocity;
}
