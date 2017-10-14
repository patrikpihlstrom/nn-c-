#pragma once

#include "application/state/State.hpp"
#include "world/World.hpp"
#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"
#include "actor/NPCSpawner.hpp"
#include "application/TextureHolder.hpp"
#include "application/Camera.hpp"
#include "object/ObjectIdTracker.hpp"
#include "object/Object.hpp"


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

