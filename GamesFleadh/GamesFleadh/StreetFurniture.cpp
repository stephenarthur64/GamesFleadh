#include "StreetFurniture.h"

StreetFurniture::StreetFurniture(bool t_hasFeeder, std::string t_furnitureType, Vector3 t_startPos)
{
	//m_position = { 1.0f, 1.0f, -13.0f };
	m_position = t_startPos;

	//m_body = LoadModel("ASSETS/3D/StreetFurniture/Mushrooms/ChunkyMushroom.glb");
	m_body = LoadModel(t_furnitureType.c_str());
	setHitBox();	

	if (t_hasFeeder)
	{
		m_feeder = new Feeder;
		m_feeder->init();
	}
}

StreetFurniture::~StreetFurniture()
{
	if (m_feeder == nullptr) return;
	delete m_feeder;
}

void StreetFurniture::rotate(int t_direction){}

void StreetFurniture::init()
{
	/*m_body = LoadModel("ASSETS/3D/StreetFurniture/Mushrooms/ChunkyMushroom.glb");
	setHitBox();
	
	if (m_feeder == nullptr) return;
	m_feeder->init();*/
}

void StreetFurniture::render()
{
	if (!m_inPlay) return; // Not in gameplay: early out.

	DrawModel(m_body, m_position, 0.5f, WHITE);
	DrawBoundingBox(m_hitbox, BLUE);

	if (m_feeder == nullptr) return;
	m_feeder->render();
}

void StreetFurniture::renderBoom(Camera& t_camera)
{
	if (m_feeder == nullptr) return;
	m_feeder->renderBoom(t_camera);
}

void StreetFurniture::playerDetected(bool t_spotted)
{
	if (m_feeder == nullptr) return;

	if (t_spotted)
	{
		m_feeder->shootBullet();
	}
	else
	{
		m_feeder->disableShooting();
	}
}

void StreetFurniture::update()
{
	if (m_feeder == nullptr) return;
	m_feeder->update();
}

void StreetFurniture::spawnEnemy()
{
	if (m_feeder == nullptr) return;
	m_feeder->spawn(m_position); 
}// Sets feeder health to 1, sets position, sets hitbox

void StreetFurniture::setHitBox()
{
	m_hitbox.min.x = m_position.x - 1.0f;
	m_hitbox.max.x = m_position.x + 1.0f;

	m_hitbox.min.y = m_position.y - 2.5;
	m_hitbox.max.y = m_position.y + 4.5f;

	m_hitbox.min.z = m_position.z - 1.0f;
	m_hitbox.max.z = m_position.z - 3.0f;
}
