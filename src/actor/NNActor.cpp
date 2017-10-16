#include "NNActor.hpp"


NNActor::NNActor() : 
	Actor()
{
	m_angle = 0;
	m_theta = M_PI*2/SENSOR_COUNT;
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(m_theta*i) - SENSOR_DISTANCE*std::sin(m_theta*i), SENSOR_DISTANCE*std::sin(m_theta*i) + SENSOR_DISTANCE*std::cos(m_theta*i));
		m_sensors.push_back(sensor);
	}

	//m_desiredSpeed = MAX_SPEED;
	//m_desiredRotationRate = MAX_ROTATION_RATE;
}

NNActor::~NNActor()
{
}

bool NNActor::isPlayer() const
{
	return false;
}

bool NNActor::isNN() const
{
	return true;
}

void NNActor::update(const float& deltaTime)
{
	control();
	updatePosition(deltaTime);
	for (int i = 0; i < m_sensors.size(); ++i)
	{
		sf::Vector2f sensor = sf::Vector2f(getPosition().x + SENSOR_DISTANCE*std::cos(m_theta*i+m_angle) - SENSOR_DISTANCE*std::sin(m_theta*i+m_angle), getPosition().y + SENSOR_DISTANCE*std::sin(m_theta*i+m_angle) + SENSOR_DISTANCE*std::cos(m_theta*i+m_angle));
		m_sensors[i] = sensor;
	}
}

void NNActor::control()
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

void NNActor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape circle;
	circle.setRadius(m_size);
	circle.setPosition({getPosition().x - m_size, getPosition().y - m_size});
	circle.setFillColor({255, 100, 255});
	target.draw(circle, states);

	for (int i = 0; i < m_sensors.size(); ++i)
	{
		sf::CircleShape circle;
		circle.setRadius(3);
		circle.setPosition(m_sensors[i]);
		circle.setFillColor({255, 100, 255});
		target.draw(circle, states);
	}
}

std::vector<sf::Vector2f> NNActor::getSensors() const
{
	return m_sensors;
}

