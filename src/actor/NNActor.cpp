#include "NNActor.hpp"


NNActor::NNActor() : 
	Actor()
{
	m_angle = M_PI*2*std::rand();
	m_theta = M_PI*2/SENSOR_COUNT;
	m_inputs.resize(SENSOR_COUNT, 0.f);
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(m_theta*i) - SENSOR_DISTANCE*std::sin(m_theta*i), SENSOR_DISTANCE*std::sin(m_theta*i) + SENSOR_DISTANCE*std::cos(m_theta*i));
		m_sensors.push_back(sensor);
	}

	m_neuralNet = NeuralNet(m_sensors);

	m_desiredSpeed = MAX_SPEED;
	m_desiredRotationRate = 0;
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
		m_inputs[i] = 0.f;
	}
}

void NNActor::control()
{
	auto decision = m_neuralNet.evaluate(m_inputs);
	for (int i = 0; i < decision.size(); ++i)
	{
		std::cout << i << ": " << decision[i] << std::endl;
		if (decision[i] >= 0.01f)
		{
			switch (i)
			{
				case 0:
					m_desiredRotationRate = -MAX_ROTATION_RATE;
				break;

				case 1:
					m_desiredRotationRate = MAX_ROTATION_RATE;
				break;

				case 2:
					m_desiredSpeed = MAX_SPEED;
				break;

				case 3:
					m_desiredSpeed = -MAX_SPEED;
				break;
			}
		}
	}

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
		circle.setRadius(4);
		circle.setPosition({m_sensors[i].x - 2, m_sensors[i].y - 2});
		circle.setFillColor({(unsigned char)(20*m_inputs[i]*10), 100, 255});
		target.draw(circle, states);
	}
}

std::vector<sf::Vector2f> NNActor::getSensors() const
{
	return m_sensors;
}

float NNActor::getInput(int index) const
{
	if (index < m_inputs.size())
	{
		return m_inputs[index];
	}

	return 0.f;
}

void NNActor::setInput(const float value, const int index)
{
	if (index < m_inputs.size() && m_inputs[index] < value)
	{
		m_inputs[index] = value;
	}
}

sf::Rect<int> NNActor::getBounds() const
{
	return {(int)getPosition().x - (int)m_size - (int)SENSOR_DISTANCE, (int)getPosition().y - (int)m_size - (int)SENSOR_DISTANCE, (int)m_size*2 + (int)SENSOR_DISTANCE*2, (int)m_size*2 + (int)SENSOR_DISTANCE*2};
}

