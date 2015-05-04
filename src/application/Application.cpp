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
	m_window.setVerticalSyncEnabled(true);

	m_camera.setSize(1600, 900);

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
		else if (event.type == sf::Event::MouseWheelMoved)
		{
			float zoom = 2 + event.mouseWheel.delta;
			m_camera.zoom(zoom/2);
		}
	}
}

void Application::update(sf::Time deltaTime)
{
	m_camera.update();
	m_map.update(m_window, m_camera);
}

void Application::render()
{
	m_window.clear(sf::Color(245, 241, 226));
	m_window.setView(m_camera);

	m_window.draw(m_map);

	m_window.display();
}

