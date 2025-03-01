#include "Grass.h"

Grass::Grass()
{
	currentState = new IdleState;
	animsCount = 0;
	animCurrentFrame = 0;
}

void Grass::init()
{
	m_body = LoadModel(FURNITURE_GRASS.c_str());
	modelAnimations = LoadModelAnimations(FURNITURE_GRASS.c_str(), &animsCount);
}

void Grass::render()
{
	DrawModel(m_body, m_position, 0.8f, WHITE);
}

void Grass::update()
{
	currentState->update(this);
}

void Grass::spawn(Vector3 t_pos)
{
	m_position = t_pos;
}
