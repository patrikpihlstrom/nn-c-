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

	m_font.loadFromFile("SourceCodePro-Regular.ttf");
	m_fpsText = sf::Text("NULL", m_font);
	m_fpsText.setCharacterSize(11);
	m_fpsText.setStyle(sf::Text::Regular);
	m_fpsText.setPosition(1500, 0);

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
			update(updateTime);
			deltaTime -= updateTime;
		}

		float fps = 1/dt.getElapsedTime().asSeconds();
		m_fpsText.setString("FPS: " + std::to_string(fps));
		m_fpsText.setPosition(m_world.getCamera().getCenter().x + m_world.getCamera().getSize().x/2 - 100, m_world.getCamera().getCenter().y - m_world.getCamera().getSize().y/2);
		m_fpsText.setColor(fps < 60 ? sf::Color(200, 50, 50):sf::Color(10, 10, 10));
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
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_window.close();
				m_running = false;
			}
		}
	}
}

void Application::update(sf::Time deltaTime)
{
	m_world.update(deltaTime.asSeconds(), m_window);
}

void Application::render()
{
	m_window.clear(sf::Color(245, 241, 226));
	m_window.setView(m_world.getCamera());

	m_window.draw(m_world);
	m_window.draw(m_fpsText);

	m_window.display();
}

