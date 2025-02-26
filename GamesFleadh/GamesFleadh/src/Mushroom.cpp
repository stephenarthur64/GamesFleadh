#include "Mushroom.h"

Mushroom::Mushroom()
{
	m_position = { 2.0f,1.0f,-15.0f };
	m_colour = RED;
}

void Mushroom::rotate(int t_direction)
{
}

void Mushroom::init()
{
	m_body = LoadModel("ASSETS/3D/StreetFurniture/Mushrooms/DefaultMushroom.glb"); // <--------- If new model, put here
	feeder.init();
}

void Mushroom::render()
{
	DrawModel(m_body, m_position, 0.5f, m_colour);
	feeder.render();
}

void Mushroom::renderBoom(Camera& t_camera)
{
	feeder.renderBoom(t_camera);
}

void Mushroom::playerDetected(bool t_spotted, Vector3 t_target)
{
	if (t_spotted)
	{
		feeder.shootBullet(t_target);
	}
	else
	{
		feeder.disableShooting();
	}
}

void Mushroom::update(Vector3 t_target)
{
	feeder.update(t_target);
}

void Mushroom::spawn(Vector3 t_pos)
{
	m_position = t_pos;
}

void Mushroom::spawnFeeder()
{
	feeder.spawn(m_position);
}
