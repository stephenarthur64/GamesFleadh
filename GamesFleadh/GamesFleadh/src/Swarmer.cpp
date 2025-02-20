#include "Swarmer.h"

Swarmer::Swarmer()
{
	currentState = new IdleState;
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
}

void Swarmer::render()
{
	DrawModel(m_body, m_position, 1.0f, WHITE);
}

void Swarmer::update()
{
	currentState->update(this);
}
