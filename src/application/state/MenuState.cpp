#include "MenuState.hpp"


MenuState::MenuState() :
	State()
{
	m_stateType = StateType::Menu;

	m_textureHolder.reset(new TextureHolder());
	m_textureHolder->loadTextures("assets/UI.lst");

	m_buttons.push_back(std::unique_ptr<Button>(new Button()));
	m_buttons.push_back(std::unique_ptr<Button>(new Button()));

	m_blurShader.loadFromFile("assets/shader/linear.fs", sf::Shader::Fragment);
}

MenuState::~MenuState()
{
}

void MenuState::enter(sf::RenderWindow& window)
{
	m_view = sf::View({(float)window.getSize().x/2, (float)window.getSize().y/2}, (sf::Vector2f)window.getSize());

	sf::Sprite sprite;
	sprite.setPosition(m_view.getSize().x/16, m_view.getSize().y/2*0.5);
	sprite.setTexture(*m_textureHolder->getTexture("resume").lock());

	m_buttons[0]->setSprite(sprite);

	sprite.setPosition(m_view.getSize().x/16, m_view.getSize().y/2*1.5 - 100);
	sprite.setTexture(*m_textureHolder->getTexture("exit").lock());
	m_buttons[1]->setSprite(sprite);

	auto image = window.capture();
	image.flipVertically();

	window.clear({188, 149, 108, 255});
	window.setView(m_view);

	m_gameStateTexture.loadFromImage(image);
	m_gameStateSprite.setTexture(m_gameStateTexture);

	m_blurShader.setParameter("texture", m_gameStateTexture);
	m_blurShader.setParameter("width", window.getSize().x);
	m_blurShader.setParameter("height", window.getSize().y);

	m_selectedIndex = 0;

	State::enter(window);
}

void MenuState::update(const float& deltaTime, const sf::RenderWindow& window)
{
	if (m_buttons[0])
	{
		m_buttons[0]->update(window);

		if (m_buttons[0]->pressed())
			sendSwitchSignal();
	}

	if (m_buttons[1])
	{
		m_buttons[1]->update(window);

		if (m_buttons[1]->pressed())
			sendExitSignal();
	}

	if (sf::Joystick::isConnected(0))
	{
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) < 0)
		{
			if (m_selectedIndex > 0)
				m_selectedIndex--;
			else
				m_selectedIndex = 0;
		}
		else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) > 0)
		{
			if (m_selectedIndex < m_buttons.size() - 1)
				m_selectedIndex++;
			else
				m_selectedIndex = m_buttons.size() - 1;
		}

		if (sf::Joystick::isButtonPressed(0, 1))
		{
			switch (m_selectedIndex)
			{
				case 0:
					sendSwitchSignal();
				break;

				case 1:
					sendExitSignal();
				break;

				default:
					std::cout << "Unrecognized button index: " << m_selectedIndex << std::endl;
				break;
			}
		}
	}

	State::update(deltaTime, window);
}

void MenuState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_gameStateSprite, &m_blurShader);

	for (int i = 0; i < m_buttons.size(); ++i)
	{
		if (sf::Joystick::isConnected(0))
		{
			if (i == m_selectedIndex)
				m_buttons[i]->setColor({255, 255, 255, 255});
			else
				m_buttons[i]->setColor({155, 155, 155, 255});
		}

		target.draw(*m_buttons[i], states);
	}
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

