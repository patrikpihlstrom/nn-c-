#include "NNActor.hpp"


NNActor::NNActor() : 
	Actor()
{
	m_size = 8.f;
	m_angle = M_PI*2*((double)std::rand()/RAND_MAX);
	m_theta = M_PI*2/SENSOR_COUNT;
	m_inputs.resize(SENSOR_COUNT, 0.f);
	m_dna.resize(SENSOR_COUNT, std::vector<float>(4, 0));
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(m_theta*i) - SENSOR_DISTANCE*std::sin(m_theta*i), SENSOR_DISTANCE*std::sin(m_theta*i) + SENSOR_DISTANCE*std::cos(m_theta*i));
		m_sensors.push_back(sensor);

		for (int j = 0; j < 4; ++j)
		{
			m_dna[i][j] = (double)std::rand()/RAND_MAX;
		}
	}

	m_neuralNet = NeuralNet(m_dna);

	m_desiredSpeed = MAX_SPEED/6.f;
	//m_desiredRotationRate = MAX_ROTATION_RATE/3.f;
}

NNActor::NNActor(const std::vector<std::vector<float>> dna) :
	Actor()
{
	m_dna = dna;
	m_size = 8.f;
	m_angle = M_PI*2*((double)std::rand()/RAND_MAX);
	m_theta = M_PI*2/SENSOR_COUNT;
	m_inputs.resize(SENSOR_COUNT, 0.f);
	for (int i = 0; i < SENSOR_COUNT; ++i)
	{
		sf::Vector2f sensor = sf::Vector2f(SENSOR_DISTANCE*std::cos(m_theta*i) - SENSOR_DISTANCE*std::sin(m_theta*i), SENSOR_DISTANCE*std::sin(m_theta*i) + SENSOR_DISTANCE*std::cos(m_theta*i));
		m_sensors.push_back(sensor);
	}

	m_neuralNet = NeuralNet(dna);

	m_desiredSpeed = MAX_SPEED/6.f;
	//m_desiredRotationRate = MAX_ROTATION_RATE/3.f;
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
	//std::cout << "Angle: " << m_angle << std::endl;
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
	if (decision.rbegin() != decision.rend() && decision.rbegin()->first >= 0.01f)
	{
		switch (decision.rbegin()->second)
		{
			case 0:
				//std::cout << "- rotation" << std::endl;
				m_desiredRotationRate = -MAX_ROTATION_RATE*decision.rbegin()->first*10;
			break;

			case 1:
				//std::cout << "+ rotation" << std::endl;
				m_desiredRotationRate = MAX_ROTATION_RATE*decision.rbegin()->first*10;
			break;

			case 2:
				//std::cout << "+ speed" << std::endl;
				m_desiredSpeed = MAX_SPEED*decision.rbegin()->first*10;
			break;

			case 3:
				//std::cout << "- speed" << std::endl;
				m_desiredSpeed = -MAX_SPEED*decision.rbegin()->first*10;
			break;
		}
	}
	else
	{
		m_desiredRotationRate = 0;
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

std::vector<std::vector<float>> NNActor::getDna() const
{
	return m_dna;
}

