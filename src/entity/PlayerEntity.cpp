#include "PlayerEntity.hpp"


PlayerEntity::PlayerEntity() :
	Entity()
{
	m_polygon.addPoint(sf::Vector2f(0, 0));
	m_polygon.addPoint(sf::Vector2f(32, 0));
	m_polygon.addPoint(sf::Vector2f(32, 32));
	m_polygon.addPoint(sf::Vector2f(0, 32));
	m_polygon.constructEdges();
	m_polygon.offset(100, 100);

	m_shape.setPointCount(m_polygon.getPointCount());
	for (int i = 0; i < m_polygon.getPointCount(); ++i)
		m_shape.setPoint(i, m_polygon.getPoint(i));

	m_shape.setFillColor(sf::Color(255, 0, 0, 255));


	m_jumpChecks[0].polygon.addPoint(sf::Vector2f(-3, 14));
	m_jumpChecks[0].polygon.addPoint(sf::Vector2f(10, 14));
	m_jumpChecks[0].polygon.addPoint(sf::Vector2f(10, 18));
	m_jumpChecks[0].polygon.addPoint(sf::Vector2f(-3, 18));
	m_jumpChecks[1].polygon.addPoint(sf::Vector2f(2, 22));
	m_jumpChecks[1].polygon.addPoint(sf::Vector2f(30, 22));
	m_jumpChecks[1].polygon.addPoint(sf::Vector2f(30, 34));
	m_jumpChecks[1].polygon.addPoint(sf::Vector2f(2, 34));
	m_jumpChecks[2].polygon.addPoint(sf::Vector2f(22, 14));
	m_jumpChecks[2].polygon.addPoint(sf::Vector2f(34, 14));
	m_jumpChecks[2].polygon.addPoint(sf::Vector2f(34, 18));
	m_jumpChecks[2].polygon.addPoint(sf::Vector2f(22, 18));

	for (int i = 0; i < 3; ++i)
	{
		m_jumpChecks[i].polygon.constructEdges();
		m_jumpChecks[i].polygon.offset(100, 100);
	}
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::control(const float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		m_maxHorizontalSpeed = 10;
	else
		m_maxHorizontalSpeed = 5;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_velocity.y < 0)
			m_velocity.y -= 0.25;

		if (m_jumpChecks[1].grounded)
			m_velocity.y = -15;
		else
		{
			if (m_jumpChecks[0].grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_velocity.y = -15 - (m_maxHorizontalSpeed == 5 ? 0:2);
				m_velocity.x = m_maxHorizontalSpeed + m_maxHorizontalSpeed/5;
			}

			if (m_jumpChecks[2].grounded && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_velocity.y = -15 - (m_maxHorizontalSpeed == 5 ? 0:2);
				m_velocity.x = -m_maxHorizontalSpeed - m_maxHorizontalSpeed/5;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_velocity.x < -m_maxHorizontalSpeed)
			m_velocity.x += .5;
		else
			m_velocity.x = m_velocity.x <= -m_maxHorizontalSpeed ? m_velocity.x:m_velocity.x - .5;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x > m_maxHorizontalSpeed)
			m_velocity.x -= .5;
		else
			m_velocity.x = m_velocity.x >= m_maxHorizontalSpeed ? m_velocity.x:m_velocity.x + .5;
	}
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ! sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (std::abs(m_velocity.x) <= 0.001)
			m_velocity.x = 0;
		else
			m_velocity.x /= 2;
	}
}

