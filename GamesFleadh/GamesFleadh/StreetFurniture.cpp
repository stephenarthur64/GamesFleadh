#include "StreetFurniture.h"

StreetFurniture::StreetFurniture()
{
	m_position = { 1.0f, 1.0f, -10.0f };
}

void StreetFurniture::rotate(int t_direction)
{
}

void StreetFurniture::init()
{
	m_body = LoadModel("ASSETS/3D/StreetFurniture/Mushrooms/ChunkyMushroom.glb");
	setHitBox();
}

void StreetFurniture::render()
{
	DrawModel(m_body, m_position, 0.5f, WHITE);
	DrawBoundingBox(m_hitbox, BLUE);
}

void StreetFurniture::setHitBox()
{
	m_hitbox.min.x = m_position.x - 1.0f;
	m_hitbox.max.x = m_position.x + 1.0f;

	m_hitbox.min.y = m_position.y - 2.5;
	m_hitbox.max.y = m_position.y + 4.5f;

	m_hitbox.min.z = m_position.z - 1.0f;
	m_hitbox.max.z = m_position.z - 3.0f;
}
