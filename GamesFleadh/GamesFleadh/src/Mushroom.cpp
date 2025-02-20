#include "Mushroom.h"

Mushroom::Mushroom()
{
	m_position = { 2.0f,1.0f,-15.0f };
	m_colour = GRAY;
}

void Mushroom::rotate(int t_direction)
{
}

void Mushroom::init()
{
	m_body = LoadModel("ASSETS/3D/StreetFurniture/Mushrooms/DefaultMushroom.glb"); // <--------- If new model, put here
	enemy.init();
}

void Mushroom::render()
{
	DrawModel(m_body, m_position, 0.5f, m_colour);
	enemy.render();
}

void Mushroom::renderBoom(Camera& t_camera)
{
	enemy.renderBoom(t_camera);
}

void Mushroom::playerDetected(bool t_spotted)
{
	if (t_spotted)
	{
		enemy.shootBullet();
	}
	else
	{
		enemy.disableShooting();
	}
}

void Mushroom::update()
{
	enemy.update();
}

void Mushroom::spawn(Vector3 t_pos)
{
	m_position = t_pos;
}

void Mushroom::spawnEnemy()
{
	enemy.spawn(m_position);
}
