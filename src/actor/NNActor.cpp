#include "NNActor.hpp"


NNActor::NNActor() : 
	Actor()
{
	m_size = 8.f;
	m_theta = M_PI/2/SENSOR_COUNT;
	m_angle = M_PI;
	m_angle = M_PI;
	m_health = 1000;
	m_inputs.resize(SENSOR_COUNT + EYE_COUNT*3 + 1, 0.f);
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		double angle = m_theta*i-(m_theta/SENSOR_COUNT);
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(angle) - SENSOR_DISTANCE*std::sin(angle), SENSOR_DISTANCE*std::sin(angle) + SENSOR_DISTANCE*std::cos(angle));
		m_sensors.push_back(sensor);
	}

	for (int i = 0; i < EYE_COUNT; ++i)
	{
		double angle = (m_theta/2)*i-((m_theta/2)/EYE_COUNT);
		sf::Vector2f sensor = sf::Vector2f(SIGHT_DISTANCE*std::cos(angle) - SIGHT_DISTANCE*std::sin(angle), SIGHT_DISTANCE*std::sin(angle) + SIGHT_DISTANCE*std::cos(angle));
		m_sensors.push_back(sensor);
	}

	m_neuralNet = NeuralNet({SENSOR_COUNT + EYE_COUNT*3 + 1, m_inputs.size()*2, 4});
	m_dna = m_neuralNet.getWeights();
	m_desiredSpeed = MAX_SPEED;
	m_desiredRotationRate = 0;
}

NNActor::NNActor(const std::vector<double> dna) :
	Actor()
{
	m_dna = dna;
	m_size = 8.f;
	m_theta = M_PI/2/SENSOR_COUNT;
	m_angle = M_PI;
	m_health = 1000;
	m_inputs.resize(SENSOR_COUNT + EYE_COUNT*3 + 1, 0.f);
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		double angle = m_theta*i-(m_theta/SENSOR_COUNT);
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(angle) - SENSOR_DISTANCE*std::sin(angle), SENSOR_DISTANCE*std::sin(angle) + SENSOR_DISTANCE*std::cos(angle));
		m_sensors.push_back(sensor);
	}

	for (int i = 0; i < EYE_COUNT; ++i)
	{
		double angle = (m_theta/2)*i-((m_theta/2)/EYE_COUNT);
		sf::Vector2f sensor = sf::Vector2f(SIGHT_DISTANCE*std::cos(angle) - SIGHT_DISTANCE*std::sin(angle), SIGHT_DISTANCE*std::sin(angle) + SIGHT_DISTANCE*std::cos(angle));
		m_sensors.push_back(sensor);
	}

	m_neuralNet = NeuralNet({SENSOR_COUNT + EYE_COUNT*3 + 1, m_inputs.size()*2, 4}, dna);
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
	m_health -= 1*deltaTime;
	if (m_health <= 0)
	{
		m_dead = true;
	}
	else
	{
		m_age += deltaTime;
		control();
		updatePosition(deltaTime);
		//std::cout << "Angle: " << m_angle << std::endl;
		for (int i = 0; i < m_sensors.size(); ++i)
		{
			double angle = m_theta*i+m_angle-(m_theta*SENSOR_COUNT/2.f);
			sf::Vector2f sensor = sf::Vector2f(getPosition().x + SENSOR_DISTANCE*std::cos(angle) - SENSOR_DISTANCE*std::sin(angle), getPosition().y + SENSOR_DISTANCE*std::sin(angle) + SENSOR_DISTANCE*std::cos(angle));
			m_sensors[i] = sensor;
			m_inputs[i] = 0.f;
		}

		for (int i = 0; i < EYE_COUNT; ++i)
		{
			double angle = (m_theta/2)*i+m_angle-((m_theta/2)*EYE_COUNT/2.f);
			sf::Vector2f eye = sf::Vector2f(getPosition().x + SIGHT_DISTANCE*std::cos(angle) - SIGHT_DISTANCE*std::sin(angle), getPosition().y + SIGHT_DISTANCE*std::sin(angle) + SIGHT_DISTANCE*std::cos(angle));
			m_sensors[SENSOR_COUNT + i] = eye;
			m_inputs[SENSOR_COUNT + i] = 0.f;
		}
	}
}

void NNActor::control()
{
	for (int i = 0; i < EYE_COUNT; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			std::cout << i << ": " << j << ": " << m_inputs[SENSOR_COUNT + i*3 + j] << std::endl;
		}
	}

	m_inputs.back() = (1000.f-m_health)/1000.f;
	m_desiredRotationRate = 0;
	auto decision = m_neuralNet.evaluate(m_inputs);
	int highestValueIndex = 0;
	for (int i = 0; i < decision.size(); ++i)
	{
		if (decision[i] > decision[highestValueIndex])
		{
			highestValueIndex = i;
		}
	}

	if (decision[highestValueIndex] >= 0.5)
	{
		switch (highestValueIndex)
		{
			case 0:
				m_desiredRotationRate = MAX_ROTATION_RATE*(decision[highestValueIndex]);
			break;

			case 1:
				m_desiredRotationRate = -MAX_ROTATION_RATE*(decision[highestValueIndex]);
			break;

			case 2:
				m_desiredSpeed = MAX_SPEED*(decision[highestValueIndex]);
			break;

			case 3:
				m_desiredSpeed = -MAX_SPEED*(decision[highestValueIndex]);
			break;
		}
	}

	//std::cout << "Desired speed: " << m_desiredSpeed << "	Desired rotation rate: " << m_desiredRotationRate << std::endl;

	Actor::control();
}

void NNActor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape circle;
	circle.setRadius(m_size);
	circle.setPosition({getPosition().x - m_size, getPosition().y - m_size});
	if (m_dead)
	{
		circle.setFillColor({255, 100, 100});
	}
	else
	{
		circle.setFillColor({255, 100, 255});
	}

	if (!m_dead)
	{
		for (int i = 0; i < m_sensors.size(); ++i)
		{
			//sf::CircleShape sensor;
			sf::Color color = {(unsigned char)(20*m_inputs[i]*10), (unsigned char)(10*m_inputs[i]*10), (unsigned char)(10*m_inputs[i]*10), 100};
			//sensor.setRadius(4);
			//sensor.setPosition({m_sensors[i].x - 2, m_sensors[i].y - 2});
			//sensor.setFillColor(color);
			sf::Vertex line[] = 
			{
				sf::Vertex(m_sensors[i], color),
				sf::Vertex(getPosition(), color)
			};
			target.draw(line, 2, sf::Lines);
			//target.draw(sensor, states);
		}
	}

	target.draw(circle, states);
}

void NNActor::drawNeuralNet(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_neuralNet, states);
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
	return {(int)getPosition().x - (int)m_size - 300, (int)getPosition().y - (int)m_size - 300, (int)m_size*2 + 600, (int)m_size*2 + 600};
}

sf::Rect<float> NNActor::getPhysicalBounds() const
{
	return {getPosition().x - m_size, getPosition().y - m_size, m_size*2, m_size*2};
}

bool NNActor::isDead() const
{
	return m_dead;
}

void NNActor::setDead(const bool dead)
{
	m_dead = dead;
}

std::vector<double> NNActor::getDna() const
{
	return m_dna;
}

void NNActor::printDna() const
{
}

sf::Vector2f NNActor::getSensor(const int index)
{
	if (index >= 0 && index < m_sensors.size())
	{
		return m_sensors[index];
	}

	return {};
}

void NNActor::setSensor(const int index, const sf::Vector2f sensor)
{
	if (index >= 0 && index < m_sensors.size())
	{
		m_sensors[index] = sensor;
	}
}

void NNActor::addHealth(const int health)
{
	m_health += health;
}

