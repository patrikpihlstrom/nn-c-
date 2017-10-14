#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor()
{
}

NPCActor::NPCActor(const std::string& npcType) :
	Actor(),
	m_npcType(npcType)
{
}

NPCActor::NPCActor(const std::string& npcType, const sf::Vector2f& position) :
	Actor(),
	m_npcType(npcType)
{
	setPositionMaster(position.x, position.y);
}

NPCActor::~NPCActor()
{
}

bool NPCActor::isPlayer() const
{
	return false;
}

bool NPCActor::isNPC() const
{
	return true;
}

void NPCActor::update(const float& deltaTime)
{
	control();
	updatePosition(deltaTime);
}

void NPCActor::control()
{
	/*if (math::distance<float>(m_target, getPosition()) <= 8)
	{
		m_target = {(float)(-500 + rand()%1000), (float)(-500 + rand()%1000)};

		float angle = math::angle<float>(m_target, getPosition());
		m_velocity.x = std::cos(angle)*600;
		m_velocity.y = std::sin(angle)*600;
	}*/

	Actor::control();
}

