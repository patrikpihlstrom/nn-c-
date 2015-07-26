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

	void enter();
	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void exit();

	sf::Vector2f getView() const;

private:
	World m_world;

	std::unique_ptr<ActorManager> m_actorManager;
	std::unique_ptr<NPCSpawner> m_npcSpawner;
	std::unique_ptr<TextureHolder> m_textureHolder;
	std::unique_ptr<ObjectIdTracker> m_objectIdTracker;
	std::unique_ptr<ActorIdTracker> m_actorIdTracker;

	std::shared_ptr<PlayerActor> m_playerActor;

	void addObject(const Object& object);
};

