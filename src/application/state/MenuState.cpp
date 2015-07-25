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
	target.setView(sf::View({(float)target.getSize().x/2, (float)target.getSize().y/2}, {(float)target.getSize().x, (float)target.getSize().y}));
	sf::RectangleShape rectShape;
	rectShape.setSize({1600, 900});
	rectShape.setFillColor({0, 255, 0});
	rectShape.setPosition(0, 0);

	target.draw(rectShape, states);
}

