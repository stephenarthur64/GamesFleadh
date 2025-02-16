#pragma once
#include <raylib.h>
#include "gameobject.h"
#include "IdleState.h"
#include "Bullet.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void setHitBox();
	void updateHitBox();
	void spawn(Vector3 t_position);
	void collision(bool t_collision);
	virtual void init() override;
	virtual void render() override;

	Model* getBulletModel(int count) { return m_mudBomb.getModel(); }
	Vector3 getBulletPositon(int count) { return m_mudBomb.getPositon(); }
	BoundingBox getBulletHitBox(int count) { return m_mudBomb.getHitbox(); }

	void shootBullet();
	void despawnBullet();
	void disableShooting();

	void kill();

	void update();

	virtual void rotate(int t_direction) override;

private:
	Bullet m_mudBomb;

	float hitboxOffsetMin = 9.0f;
	float hitboxOffsetMax = 11.0f;

	int bulletTick = -1;
};

