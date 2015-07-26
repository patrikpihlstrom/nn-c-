#include "application/state/MenuState.hpp"


MenuState::MenuState() :
	State()
{
	m_stateType = StateType::Menu;
}

MenuState::~MenuState()
{
}

void MenuState::enter()
{
	State::enter();
}

void MenuState::update(const float& deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		m_signalStack.push_back(Signal::Exit);

	State::update(deltaTime);
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void MenuState::exit()
{
	State::exit();
}

