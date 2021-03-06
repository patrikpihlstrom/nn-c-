#include "application/state/State.hpp"


State::State()
{
	m_escape = false;
}

State::~State()
{
}

void State::enter(sf::RenderWindow& window)
{
	m_escape = true;
}

void State::update(const float& deltaTime, const sf::RenderWindow& window)
{
	if (!m_escape && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		m_signalStack.push_back(Signal::Switch);

	m_escape = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
}

void State::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void State::exit()
{
}

StateType State::getStateType() const
{
	return m_stateType;
}

std::vector<Signal> State::getSignals() const
{
	return m_signalStack;
}

void State::clearSignals()
{
	m_signalStack.clear();
}

sf::Vector2f State::getView() const
{
	return {0, 0};
}

