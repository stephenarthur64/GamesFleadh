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
}

void StreetFurniture::render()
{
	DrawModel(m_body, m_position, 0.5f, WHITE);
}

void StreetFurniture::setHitBox()
{
}
