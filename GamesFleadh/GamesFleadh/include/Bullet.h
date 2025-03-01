#pragma once
#include <raylib.h>
#include <raymath.h>
#include "gameobject.h"

class Bullet : public GameObject
{
public:
	Bullet();
	void setHitBox();

	float getRadius() { return m_radius; }

	void spawn(Vector3 t_pos, float speed, Vector3 t_velocity);
	void despawn();
	void move();
	void follow(Vector3 t_target);
	void init(int t_type);
	virtual void init() override {};
	virtual void render() override;
	virtual void rotate(int t_direction) override;

private:
	float m_speed;
	bool m_active;
	Vector3 m_velocity;
	Vector3 m_direction;
	float distance;
	float m_radius;
};

