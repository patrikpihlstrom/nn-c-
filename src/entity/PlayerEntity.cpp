#include "PlayerEntity.hpp"


PlayerEntity::PlayerEntity() :
	Entity()
{
	m_polygon.addPoint(sf::Vector2f(0, 0));
	m_polygon.addPoint(sf::Vector2f(32, 0));
	m_polygon.addPoint(sf::Vector2f(32, 32));
	m_polygon.addPoint(sf::Vector2f(0, 32));
	m_polygon.constructEdges();

	m_shape.setPointCount(m_polygon.getPointCount());
	for (int i = 0; i < m_polygon.getPointCount(); ++i)
		m_shape.setPoint(i, m_polygon.getPoint(i));

	m_shape.setFillColor(sf::Color(255, 0, 0, 255));


	m_checks[0].addPoint(sf::Vector2f(-3, 14));
	m_checks[0].addPoint(sf::Vector2f(10, 14));
	m_checks[0].addPoint(sf::Vector2f(10, 18));
	m_checks[0].addPoint(sf::Vector2f(-3, 18));
	m_checks[1].addPoint(sf::Vector2f(2, 22));
	m_checks[1].addPoint(sf::Vector2f(30, 22));
	m_checks[1].addPoint(sf::Vector2f(30, 34));
	m_checks[1].addPoint(sf::Vector2f(2, 34));
	m_checks[2].addPoint(sf::Vector2f(22, 14));
	m_checks[2].addPoint(sf::Vector2f(34, 14));
	m_checks[2].addPoint(sf::Vector2f(34, 18));
	m_checks[2].addPoint(sf::Vector2f(22, 18));

	for (int i = 0; i < 3; ++i)
		m_checks[i].constructEdges();
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::control(const float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		m_horizontalSpeed = 10;
	else
		m_horizontalSpeed = 5;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_grounded[1])
			m_velocity.y = -15;
		else
		{
			if (m_grounded[0] && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				m_velocity.y = -15 - (m_horizontalSpeed == 5 ? 0:2);
				m_velocity.x = m_horizontalSpeed + m_horizontalSpeed/5;
			}

			if (m_grounded[2] && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				m_velocity.y = -15 - (m_horizontalSpeed == 5 ? 0:2);
				m_velocity.x = -m_horizontalSpeed - m_horizontalSpeed/5;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_velocity.x < -m_horizontalSpeed)
			m_velocity.x += .5;
		else
			m_velocity.x = m_velocity.x <= -m_horizontalSpeed ? m_velocity.x:m_velocity.x - .5;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x > m_horizontalSpeed)
			m_velocity.x -= .5;
		else
			m_velocity.x = m_velocity.x >= m_horizontalSpeed ? m_velocity.x:m_velocity.x + .5;
	}
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && ! sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (std::abs(m_velocity.x) <= 0.001)
			m_velocity.x = 0;
		else
			m_velocity.x /= 2;
	}
}

