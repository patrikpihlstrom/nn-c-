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
	settings.antialiasingLevel = 32;

	m_window.create(sf::VideoMode(1600, 900), "Editor", sf::Style::Close, settings);
	m_window.setVerticalSyncEnabled(true);
	m_window.setFramerateLimit(71);

	m_camera.reset(sf::FloatRect(0, 0, 1600, 900));

	m_player.reset(new PlayerEntity());
	m_map.load("level.obj");
	m_player->setQuadtree(m_map.getQuadtree());
	m_camera.trackEntity(m_player);

	m_shadowCaster.setQuadtree(m_map.getQuadtree());
	m_shadowCaster.updateShape();

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

		while (deltaTime > updateTime)
		{
			update(updateTime);
			deltaTime -= updateTime;
		}

		render();
	}
}

void Application::handleEvents()
{
	sf::Event event;

	while(m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_running = false;
		else if (event.type == sf::Event::KeyPressed)
		{

		}
	}
}

void Application::update(sf::Time p_deltaTime)
{
	m_player->update(p_deltaTime.asSeconds());
	m_camera.update();
	m_shadowCaster.bounds->offset(m_player->getPosition().x - m_shadowCaster.bounds->getCenter().x, m_player->getPosition().y - m_shadowCaster.bounds->getCenter().y);
	m_shadowCaster.setPosition(m_player->getPosition());
	//m_shadowCaster.m_bounds->offset((sf::Mouse::getPosition(m_window).x + m_camera.getCenter().x - 800) - m_shadowCaster.m_bounds->getCenter().x, (sf::Mouse::getPosition(m_window).y + m_camera.getCenter().y - 450) - m_shadowCaster.m_bounds->getCenter().y);
	m_shadowCaster.updateShape();
}

void Application::render()
{
	m_window.setView(m_camera);
	m_window.clear(sf::Color(46, 46, 46));

	m_window.draw(m_shadowCaster);
	m_window.draw(m_map);
	m_window.draw(*m_player);

	m_window.display();
}

