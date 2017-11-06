#include "NNActor.hpp"


NNActor::NNActor()
{
	m_size = 8.f;
	m_age = 0;
	m_theta = 2*M_PI/SENSOR_COUNT;
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
		double angle = (M_PI/25)*i-((M_PI/25)/EYE_COUNT);
		sf::Vector2f sensor = sf::Vector2f(SIGHT_DISTANCE*std::cos(angle) - SIGHT_DISTANCE*std::sin(angle), SIGHT_DISTANCE*std::sin(angle) + SIGHT_DISTANCE*std::cos(angle));
		m_sensors.push_back(sensor);
	}

	m_neuralNet = NeuralNet((NeuralNet::Settings){SENSOR_COUNT + EYE_COUNT*3 + 1, m_inputs.size()*2, 4});
	m_dna = m_neuralNet.getWeights();
	m_desiredSpeed = MAX_SPEED;
	m_desiredRotationRate = 0;
}

NNActor::NNActor(const std::vector<double> dna)
{
	m_dna = dna;
	m_age = 0;
	m_size = 8.f;
	m_theta = 2*M_PI/SENSOR_COUNT;
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
		double angle = (M_PI/25)*i-((M_PI/25)/EYE_COUNT);
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

void NNActor::update(const float& deltaTime)
{
	m_health -= 1*deltaTime;
	m_age += 1*deltaTime;
	control();
	sf::Transformable::move(m_velocity.x*deltaTime, m_velocity.y*deltaTime);
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
		double angle = (M_PI/25)*i+m_angle-((M_PI/25)*EYE_COUNT/2.f);
		sf::Vector2f eye = sf::Vector2f(getPosition().x + SIGHT_DISTANCE*std::cos(angle) - SIGHT_DISTANCE*std::sin(angle), getPosition().y + SIGHT_DISTANCE*std::sin(angle) + SIGHT_DISTANCE*std::cos(angle));
		m_sensors[SENSOR_COUNT + i] = eye;
		m_inputs[SENSOR_COUNT + i] = 0.f;
	}
}

void NNActor::control()
{
	m_inputs.back() = (1000.f-m_health)/1000.f;
	m_desiredRotationRate = 0;
	m_rotationRate = 0;
	auto decision = m_neuralNet.evaluate(m_inputs);
	m_inputs.resize(SENSOR_COUNT + EYE_COUNT*3 + 1, 0.f);
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

	float speed = math::magnitude<float>(m_velocity);
	float deltaSpeed = m_desiredSpeed - speed;
	if (std::abs(deltaSpeed) > MAX_ACC)
	{
		if (deltaSpeed < 0)
		{
			deltaSpeed = -MAX_ACC;
		}
		else
		{
			deltaSpeed = MAX_ACC;
		}
	}

	float targetSpeed = speed + deltaSpeed;
	m_velocity = {targetSpeed*std::cos(m_angle) - targetSpeed*std::sin(m_angle), targetSpeed*std::sin(m_angle) + targetSpeed*std::cos(m_angle)};

	speed = math::magnitude<float>(m_velocity);
	if (speed > MAX_SPEED)
	{
		math::normalize(m_velocity);
		m_velocity.x *= MAX_SPEED;
		m_velocity.y *= MAX_SPEED;
	}

	float deltaRotationRate = m_desiredRotationRate - m_rotationRate;
	if (std::abs(deltaRotationRate) > MAX_ROTATION_ACC)
	{
		if (deltaRotationRate < 0)
		{
			deltaRotationRate = -MAX_ROTATION_ACC;
		}
		else
		{
			deltaRotationRate = MAX_ROTATION_ACC;
		}
	}

	m_rotationRate += deltaRotationRate;
	if (std::abs(m_rotationRate) > MAX_ROTATION_RATE)
	{
		if (m_rotationRate < 0)
		{
			m_rotationRate = -MAX_ROTATION_RATE;
		}
		else
		{
			m_rotationRate = MAX_ROTATION_RATE;
		}
	}

	m_angle += m_rotationRate;
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

	/*if (!m_dead)
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
	}*/

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

double NNActor::getInput(int index) const
{
	if (index < m_inputs.size())
	{
		return m_inputs[index];
	}

	return 0.f;
}

void NNActor::setInput(const double value, const int index)
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

int NNActor::getHealth() const
{
	return m_health;
}

void NNActor::setHealth(const uint8_t health)
{
	m_health = health;
}

void NNActor::addHealth(const int health)
{
	m_health += health;
}

ActorId NNActor::getId() const
{
	return m_id;
}

void NNActor::assign(const ActorId& id)
{
	m_id = id;
}

bool NNActor::operator<(const NNActor& actor) const
{
	return getPosition().y + m_size < actor.getPosition().y + actor.getSize();
}

float NNActor::getSize() const
{
	return m_size;
}

void NNActor::setSize(const float size)
{
	m_size = size;
}

float NNActor::getAge() const
{
	return m_age;
}

