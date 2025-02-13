#include "Mushroom.h"

Mushroom::Mushroom()
{
	m_position = { 2.0f,2.0f,-15.0f };
	m_colour = GRAY;
}

void Mushroom::rotate(int t_direction)
{

}

void Mushroom::init()
{
	m_body = LoadModel("ASSETS/RS/cube.glb"); // <--------- If new model, put here
	enemy.init();
}

void Mushroom::render()
{
	DrawModel(m_body, m_position, 2.0f, m_colour);
	enemy.render();
}

void Mushroom::update()
{
	enemy.update();
}

void Mushroom::spawnEnemy()
{
	enemy.spawn(m_position);
}
