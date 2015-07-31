#pragma once

#include "application/state/State.hpp"
#include "world/World.hpp"
#include "application/TextureHolder.hpp"


class GameState : public State
{
public:
	GameState();
	~GameState();

	void enter(sf::RenderWindow& window);
	void update(const float& deltaTime, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void exit();

private:
	World m_world;

	std::unique_ptr<TextureHolder> m_textureHolder;
};

