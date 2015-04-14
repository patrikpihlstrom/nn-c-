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
	m_camera.reset(sf::FloatRect(0, 0, 1600, 900));
	m_window.setVerticalSyncEnabled(true);

	m_player.reset(new PlayerEntity());
	m_map.load("level.obj");
	m_player->setQuadtree(m_map.getQuadtree());
	m_camera.trackEntity(m_player);

	m_running = true;
	m_active = true;
}

void Application::loadMap(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);

	if (file.is_open())
	{
		std::string line;
		math::Polygon polygon;
		while (!file.eof())
		{
			std::getline(file, line);
			if (line.size() == 1 && line[0] == '-') // new polygon
			{
				polygon.constructEdges();
				m_map.addPolygon(polygon);
				polygon.clear();
			}
			else
			{
				std::string x, y;
				bool _x = true;

				for (int i = 0; i < line.size(); ++i)
				{
					if (line[i] == ':')
					{
						_x = false;
						++i;
					}

					if (_x)
						x += line[i];
					else
						y += line[i];
				}

				polygon.addPoint(sf::Vector2f(std::atoi(x.c_str()), std::atoi(y.c_str())));
			}
		}

		file.close();
	}
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
		render();
		while (deltaTime >= updateTime && m_active)
		{
			update(deltaTime);
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
			m_running = false;
		else if (event.type == sf::Event::KeyPressed)
		{

		}
	}
}

void Application::update(sf::Time & p_deltaTime)
{
	m_player->update(p_deltaTime.asSeconds());
	m_camera.update();
}

void Application::render()
{
	m_window.setView(m_camera);
	m_window.clear(sf::Color(46, 46, 46));

	m_window.draw(m_map);
	m_window.draw(*m_player);

	m_window.display();
}

