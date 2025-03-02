#include "Player.h"


Player::Player() : m_speed(0.2f),  bulletCount(0), HEALTHBAR_MAX(450), m_poisoned(false), m_poisonTick(-1), MAX_POISON_TICK(30),
					m_hpColour(GREEN)
{
	currentState = new NoInputState;
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

	m_currentVelocity = t_velocity;
}

void Player::updateLimits(Vector2 t_low, Vector2 t_high)
{
	upperLimit = t_high;
	lowerLimit = t_low;
}

void Player::reboundLimits(Vector3 &t_cam)
{
	lowerLimit = { t_cam.x - 1.0f, t_cam.y - 2.0f };
	upperLimit = { t_cam.x + 1.0f, t_cam.y - 2.0f };
}

void Player::setHitBox()
{
	Vector3 minOffset = { -hitboxOffsetMin, -0.1f, -1.0f };
	Vector3 maxOffset = { hitboxOffsetMax, 1.1f, 0.5f };

	m_hitbox.min = m_position + minOffset;
	m_hitbox.max = m_position + maxOffset;
}

void Player::updateHitBox(float t_z)
{
	m_hitbox.min.z += t_z;
	m_hitbox.max.z += t_z;
}

void Player::worldCollision(bool collide)
{
	if (collide)
	{
		handleInput(Event::EVENT_DAMAGE);
		if(m_health > 0) m_health -= 10;
	}
}

void Player::enemyCollision(bool collide)
{
	if (collide)
	{
		handleInput(Event::EVENT_DAMAGE);
		m_health -= 10;
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

	shootingSFX = LoadSound("ASSETS/Audio/SFX/Buzz/buzzHasThatThangOnHim.mp3");
	SetSoundVolume(shootingSFX, 0.3);

	environmentHitSFX = LoadSound("ASSETS/Audio/SFX/Buzz/buzzEnvironmentHitRedux.mp3");
	SetSoundVolume(environmentHitSFX, 0.3);

	enemyHitSFX = LoadSound("ASSETS/Audio/SFX/Buzz/buzzGetHitRedux.mp3");
	SetSoundVolume(enemyHitSFX, 0.3);

	m_position.y += 2.0f;

	m_healthbar.x = 40;
	m_healthbar.y = 1015;
	m_healthbar.width = HEALTHBAR_MAX;
	m_healthbar.height = 40;

	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].init(0);
	}

	m_boundingBoxRadius = boundingBoxRadius(getHitbox());
}

void Player::render()
{
	DrawModel(m_body, m_position, 2.0f, m_colour);

	for (int i = 0; i < getBulletMax(); i++)
	{
		bullet[i].render();
	}

	if (g_renderWireDebug)
	{
		DrawBoundingBox(m_hitbox, RED);
		DrawLine3D(g_furnCollisionItem, g_furnCollisionPlyr, PURPLE);
	}
}

void Player::shootSound()
{
	PlaySound(shootingSFX);
}

void Player::hitSound(int t_type)
{
	if (t_type == 0)
	{
		if (IsSoundPlaying(environmentHitSFX)) return;
		PlaySound(environmentHitSFX);
	}
	else if (t_type == 1)
	{
		if (IsSoundPlaying(enemyHitSFX)) return;
		PlaySound(enemyHitSFX);
	}
}

void Player::update(Vector3 &t_cam, Vector3 &t_crosshair)
{
	currentState->update(this);
	updateHealthbar();
	if (m_poisoned && m_health > 10 && m_poisonTick > MAX_POISON_TICK)
	{
		m_health -= 5;
		m_poisonTick = 0;
	}
	else if (m_health <= 10)
	{
		m_poisoned = false;
		m_hpColour = GREEN;
	}
	else
	{
		m_poisonTick++;
	}

	if (m_reboundCounter > 0)
	{
		float frameTime = GetFrameTime();
		m_reboundCounter -= frameTime;
		Vector3 rebound = m_reboundDirection * m_reboundForce * frameTime;
		m_position += rebound;
		t_crosshair += rebound;
		t_cam += rebound;
		upperLimit.x += rebound.x;
		upperLimit.y += rebound.y;

		lowerLimit.x += rebound.x;
		lowerLimit.y += rebound.y;
	}
	/*else if (m_reboundCounter < 0 && m_reboundCounter != -100)
	{
		reboundLimits(t_cam);
		m_reboundCounter = -100;
	}*/


	m_position.y = Clamp(m_position.y, 1.0f, 10.0f);
	//t_cam.y = Clamp(m_position.y, -10.0f, 15.0f);
	if (t_cam.y > 10.0f)
	{
		t_cam.y = 10.0f;
	}
	//std::cout << "Y position is: " << m_position.y << "\n";

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
		bullet[bulletCount].spawn(m_position, 0.8f, t_target);
		bulletCount++;
	}
	else
	{
		bulletCount = 0;
		bullet[bulletCount].spawn(m_position, 0.8f, t_target);
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

void Player::rebound(Vector3 t_impactPoint)
{
	std::cout << "Rebound triggered.\n";
	m_reboundCounter = m_reboundCountMax;

	m_reboundDirection = Vector3Normalize(m_position - t_impactPoint);
	m_reboundDirection *= 2.5f;
	if (m_position.y < 1.0f)
	{
		m_reboundDirection.y = 0.5f;
	}
	else
	{
		m_reboundDirection.y = 0.0f;
	}	
	m_reboundCrosshair = m_reboundDirection;
}

void Player::reboundFurniture(FurnitureCollisionData t_data)
{
	m_auto = false;
	std::cout << "Rebound triggered.\n";
	if (m_currentVelocity == Vector3{0.0f, 0.0f, 0.0f})
	{
		m_currentVelocity.z = 2.0f;
	}

	m_reboundCounter = m_reboundCountMax;

	Vector3 normal = Vector3Normalize(t_data.lastFurnitureCollision - m_position);

	m_position.z += normal.z * 2.0f;

	//m_reboundDirection = Vector3Reflect(m_currentVelocity, normal);
	////m_reboundDirection.y = 0.0f;
	//m_position.x = t_data.lastFurnitureCollision.x + normal.x * (t_data.lastFurnitureRadius + 0.1f);
	//m_position.z = t_data.lastFurnitureCollision.z + normal.z * (t_data.lastFurnitureRadius + 0.1f);
}

void Player::poisonPlayer(bool t_poison)
{
	m_poisoned = t_poison;
	m_poisonTick = 0;
	m_hpColour = ORANGE;
}

