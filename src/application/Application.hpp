#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <cstdlib>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32 ||_WIN64
#include <sys/types.h>
#endif

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "state/State.hpp"
#include "state/MenuState.hpp"
#include "state/GameState.hpp"


class Application
{
public:
	Application(int argc, char** argv);
	~Application();
	
private:
	void run();
	void initialize();
	void handleEvents();
	void update(const float& deltaTime);
	void render();
	void switchStates();

	bool m_running, m_active;

	std::unique_ptr<State> m_currentState;
	std::unique_ptr<State> m_previousState;

	sf::Font m_font;
	sf::Text m_fpsText;

	sf::RenderWindow m_window;

	sf::Texture m_gameStateTexture;
	sf::Sprite m_gameStateSprite;
	sf::Shader m_blurShader;

	void loadSettingsFile(const std::string& path);
};

