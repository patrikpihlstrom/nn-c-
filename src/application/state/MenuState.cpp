#include "application/state/MenuState.hpp"


MenuState::MenuState() :
	State()
{
	m_stateType = StateType::Menu;

	m_textureHolder.reset(new TextureHolder());
	m_textureHolder->loadTextures("assets/UI.lst");

	m_resume.reset(new Button());
	m_exit.reset(new Button());

	sf::Sprite sprite;

	sprite.setPosition(100, 900/3 - 100);
	sprite.setTexture(*m_textureHolder->getTexture("resume").lock());
	m_resume->setSprite(sprite);

	sprite.setPosition(100, 600);
	sprite.setTexture(*m_textureHolder->getTexture("exit").lock());
	m_exit->setSprite(sprite);

	m_blurShader.loadFromFile("assets/shader/linear.fs", sf::Shader::Fragment);

	m_view = sf::View({1600/2, 900/2}, {1600, 900});
}

MenuState::~MenuState()
{
}

void MenuState::enter(sf::RenderWindow& window)
{
	auto image = window.capture();
	image.flipVertically();

	window.clear({188, 149, 108, 255});
	window.setView(m_view);

	m_gameStateTexture.loadFromImage(image);
	m_gameStateSprite.setTexture(m_gameStateTexture);

	m_blurShader.setParameter("texture", m_gameStateTexture);

	State::enter(window);
}

void MenuState::update(const float& deltaTime, const sf::RenderWindow& window)
{
	if (m_resume)
	{
		m_resume->update(window);

		if (m_resume->pressed())
			sendSwitchSignal();
	}

	if (m_exit)
	{
		m_exit->update(window);

		if (m_exit->pressed())
			sendExitSignal();
	}

	State::update(deltaTime, window);
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_gameStateSprite, &m_blurShader);

	if (m_resume)
		target.draw(*m_resume, states);

	if (m_exit)
		target.draw(*m_exit, states);
}

void MenuState::exit()
{
	State::exit();
}

void MenuState::sendSwitchSignal()
{
	m_signalStack.clear();
	m_signalStack.push_back(Signal::Switch);
}

void MenuState::sendExitSignal()
{
	m_signalStack.clear();
	m_signalStack.push_back(Signal::Exit);
}

