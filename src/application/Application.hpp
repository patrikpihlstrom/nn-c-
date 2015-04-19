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

#include "Math.hpp"
#include "../map/Map.hpp"
#include "../entity/PlayerEntity.hpp"
#include "../light/ShadowCaster.hpp"
#include "Camera.hpp"


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

	bool m_mouse;
	int m_index;

	Map m_map;
	ShadowCaster m_shadowCaster;

	std::shared_ptr<PlayerEntity> m_player;

	sf::RenderWindow m_window;
	Camera m_camera;
};

