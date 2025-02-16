#include "Weapon.h"

Weapon::Weapon()
{
	currentState = new IdleState;
	animsCount = 0;
	animCurrentFrame = 0;
	modelAnimations = LoadModelAnimations("ASSETS/3D/Player/Weapons/Weapons.glb", &animsCount);
}

void Weapon::rotate(int t_direction)
{
}

void Weapon::init()
{
	m_body = LoadModel("ASSETS/3D/Player/Weapons/Weapons.glb");
}

void Weapon::render()
{
	DrawModel(m_body, m_position, 2.0f, WHITE);
}

void Weapon::update(Vector3 t_newPos)
{
	m_position = t_newPos;
	currentState->update(this);
}
