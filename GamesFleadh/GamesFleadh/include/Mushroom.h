#pragma once
#include "gameobject.h"
#include "Feeder.h"

class Mushroom : public GameObject
{
public:
	Mushroom();
	virtual void rotate(int t_direction) override;
	virtual void init() override;
	virtual void render() override;

	void renderBoom(Camera& t_camera);

	BoundingBox getFeederHitbox() { return feeder.getHitbox(); }
	void setCollisions(bool t_collide) { feeder.collision(t_collide); }
	GameObject* getFeeder() { return &feeder; }

	void playerDetected(bool t_spotted);

	bool isActive() { return feeder.isActive(); }

	void update();

	void spawn(Vector3 t_pos);
	void spawnFeeder();

private:
	Feeder feeder;
};

