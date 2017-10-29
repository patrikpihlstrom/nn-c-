#include "PlayerActor.hpp"

PlayerActor::PlayerActor() :
	Actor()
{
	m_camera.reset(new Camera());
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::control()
{
	if (sf::Joystick::isConnected(0))
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) != 0)
			m_velocity.x = TERMINAL_X_WALK*(float)(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X)/100);

		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) != 0)
			m_velocity.y = TERMINAL_Y_WALK*(float)(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y)/100);

		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) != 0)
			m_velocity.x = TERMINAL_X_WALK*(float)(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX)/100);

		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) != 0)
			m_velocity.y = TERMINAL_Y_WALK*(float)(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY)/100);

		if (sf::Joystick::isButtonPressed(0, 6) || sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
			m_camera->zoom(.9f);
		else if (sf::Joystick::isButtonPressed(0, 7) || sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
			m_camera->zoom(1.1f);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
		m_camera->zoom(.9f);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
		m_camera->zoom(1.1f);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (m_velocity.y > sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? -TERMINAL_Y_RUN:-TERMINAL_Y_WALK)
			m_velocity.y -= 10;
		else
			m_velocity.y = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? -TERMINAL_Y_RUN:-TERMINAL_Y_WALK;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (m_velocity.y < sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_Y_RUN:TERMINAL_Y_WALK)
			m_velocity.y += 10;
		else
			m_velocity.y = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_Y_RUN:TERMINAL_Y_WALK;
	}
	else
		m_velocity.y = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (m_velocity.x > sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? -TERMINAL_X_RUN:-TERMINAL_X_WALK)
			m_velocity.x -= 10;
		else
			m_velocity.x = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? -TERMINAL_X_RUN:-TERMINAL_X_WALK;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (m_velocity.x < sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_X_RUN:TERMINAL_X_WALK)
			m_velocity.x += 10;
		else
			m_velocity.x = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_X_RUN:TERMINAL_X_WALK;
	}
	else
		m_velocity.x = 0;

	if (math::magnitude<float>(m_velocity) > TERMINAL_X_WALK)
	{
		math::normalize(m_velocity);
		m_velocity.x *= sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_X_RUN:TERMINAL_X_WALK;
		m_velocity.y *= sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? TERMINAL_Y_RUN:TERMINAL_Y_WALK;
	}

	Actor::control();
}

void PlayerActor::update(const float& deltaTime)
{
	control();
	m_camera->update(getPosition(), {0, 0});

	Actor::update(deltaTime);
}

void PlayerActor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Actor::draw(target, states);
}

bool PlayerActor::isPlayer() const
{
	return true;
}

bool PlayerActor::isNPC() const
{
	return false;
}

std::weak_ptr<Camera> PlayerActor::getCamera() const
{
	return m_camera;
}

