#include "Application.hpp"

Application::Application(int argc, char** argv)
{
	run();
}

Application::~Application()
{

}

void Application::initialize()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	m_window.create(sf::VideoMode(1600, 900), "Editor", sf::Style::Close, settings);
	m_window.setFramerateLimit(120);
	//m_window.setPosition({1600 + (1920 - 1600)/2, (1080 - 900)/2});

	if (!m_font.loadFromFile("SourceCodePro-Regular.ttf"))
		std::cout << "Error loading font." << std::endl;

	m_fpsText = sf::Text("NULL", m_font);
	m_fpsText.setCharacterSize(11);
	m_fpsText.setStyle(sf::Text::Regular);
	m_fpsText.setPosition(1500, 0);

	m_currentState.reset(new GameState());
	m_currentState->enter(m_window);

	m_running = true;
	m_active = true;
}

void Application::run()
{
	initialize();
	
	sf::Clock dt;
	sf::Time deltaTime;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while(m_running)
	{
		deltaTime += dt.getElapsedTime();
		dt.restart();

		handleEvents();

		if (deltaTime > sf::seconds(.2f))
			deltaTime = sf::seconds(.2f);

		render();
		while (deltaTime > updateTime)
		{
			update(updateTime.asSeconds());
			deltaTime -= updateTime;
		}
	}
}

void Application::handleEvents()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
			m_running = false;
		}
	}
}

void Application::update(const float& deltaTime)
{
	if (m_currentState)
	{
		auto signals = m_currentState->getSignals();

		for (int i = 0; i < signals.size(); ++i)
		{
			switch (signals[i])
			{
				case Signal::Exit:
					m_running = false;
					return;
				break;

				case Signal::Switch:
					switchStates();
				break;
			}
		}

		m_currentState->clearSignals();
		m_currentState->update(deltaTime, m_window);
	}
}

void Application::render()
{
//	m_window.clear(sf::Color(245, 241, 226));
	m_window.clear(sf::Color(188, 149, 108));

	if (m_currentState)
		m_window.draw(*m_currentState);

	m_window.display();
}

void Application::switchStates()
{
	if (m_currentState)
		m_currentState->exit();

	if (!m_previousState)
	{
		switch (m_currentState->getStateType())
		{
			case StateType::Menu:
				m_previousState.swap(m_currentState);
				m_currentState.reset(new GameState());
			break;

			case StateType::Game:
				m_previousState.swap(m_currentState);
				m_currentState.reset(new MenuState());
			break;

			default:
				std::cout << "Unrecognized state type: " << (int)(m_currentState->getStateType()) << std::endl;
			break;
		}
	}
	else
		m_previousState.swap(m_currentState);

	m_currentState->enter(m_window);
}

