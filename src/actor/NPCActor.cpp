#include "actor/NPCActor.hpp"


NPCActor::NPCActor() : 
	Actor(),
	m_grounded(false)
{
	m_target = {(float)(-500 + rand()%1000), (float)(-500 + rand()%1000)};

	float angle = rand()%360;
	angle = math::toRadians<float>(angle);

	m_velocity.x = std::cos(angle)*600;
	m_velocity.y = std::sin(angle)*600;
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

void NPCActor::control()
{
	/*if (math::distance<float>(m_target, getPosition()) <= 8)
		m_target = {(float)(-5000 + rand()%10000), (float)(-500 + rand()%1000)};

	float angle = math::angle<float>(m_target, getPosition());
	m_velocity.x = std::cos(angle)*600;
	m_velocity.y = std::sin(angle)*600;
	*/

	if ((getBounds().left <= -500 && m_velocity.x < 0) || (getBounds().left + getBounds().width >= 500 && m_velocity.x > 0))
		m_velocity.x *= -.8;
	if ((getBounds().top <= -5000 && m_velocity.y < 0) || (getBounds().top + getBounds().height >= 500 && m_velocity.y > 0))
	{
		if (std::abs(m_velocity.y) >= 100)
			m_velocity.y *= -.5;
		else if (getBounds().top + getBounds().height >= 500)
		{
			m_grounded = true;
			m_velocity.y = 0;
		}
	}

	if (!m_grounded)
		m_velocity.y += 9.82*2;
	else
		m_velocity.x *= 0.9;
}

