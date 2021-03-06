#include "Actor.hpp"

Actor::Actor()
{
	m_distance = 0;
	m_dead = false;
	m_angle = 0.f;
}

Actor::~Actor()
{
}

void Actor::update(const float& deltaTime)
{
	control();
	updatePosition(deltaTime);
}

void Actor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::CircleShape circle;
	circle.setRadius(m_size);
	circle.setPosition(getPosition());
	circle.setFillColor({255, 100, 255});
	target.draw(circle, states);
}



void Actor::damage(const int8_t& factor)
{
	if (m_health - factor >= 0)
		m_health -= factor;
	else
		m_health = 0;
}

void Actor::control()
{
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

void Actor::updatePosition(const float& deltaTime)
{
	m_distance += math::magnitude<float>({m_velocity.x*deltaTime, m_velocity.y*deltaTime});
	move(m_velocity.x*deltaTime, m_velocity.y*deltaTime);
}

void Actor::move(const float x, const float y)
{
	Transformable::move(x, y);
}

ActorId Actor::getId() const
{
	return m_id;
}

void Actor::assign(const ActorId& id)
{
	m_id = id;
}

uint64_t Actor::getIdAsInt() const
{
	return m_id.id;
}

bool Actor::operator<(const Actor& actor) const
{
	return getPosition().y + m_size < actor.getPosition().y + actor.getSize();
}

float Actor::getSize() const
{
	return m_size;
}

void Actor::setSize(const float size)
{
	m_size = size;
}

sf::Rect<int> Actor::getBounds() const
{
	return {(int)getPosition().x - (int)m_size, (int)getPosition().y - (int)m_size, (int)m_size*2, (int)m_size*2};
}

float Actor::getDistance() const
{
	return m_distance;
}



