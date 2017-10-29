#include "GameState.hpp"


GameState::GameState() :
	State()
{
	m_stateType = StateType::Game;

	m_textureHolder.reset(new TextureHolder());
	m_textureHolder->loadTextures("assets/Textures.lst");

	m_world = World(time(NULL));
	m_world.load("map.obj");
}

GameState::~GameState()
{
}

void GameState::enter(sf::RenderWindow& window)
{
	State::enter(window);
}

void GameState::update(const float& deltaTime, sf::RenderWindow& window)
{
	m_world.update(deltaTime);
	window.setView(m_world.getView());
	State::update(deltaTime, window);
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_world, states);
}

void GameState::exit()
{
	State::exit();
}

