#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <chrono>
#include <unistd.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "world/World.hpp"
#include "application/Camera.hpp"
#include "ai/StateMachine.hpp"


class Application
{
public:
	Application(int argc, char** argv);
	~Application();
	
private:
	void run();
	void initialize();
	void handleEvents();
	void update(sf::Time deltaTime);
	void render();

	bool m_running, m_active;

	sf::Font m_font;
	sf::Text m_fpsText;

	std::unique_ptr<World> m_world;

	sf::RenderWindow m_window;
};

