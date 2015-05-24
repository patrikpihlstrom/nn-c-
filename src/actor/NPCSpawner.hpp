#pragma once

#include <string>

#include "actor/ActorManager.hpp"
#include "actor/ActorIdTracker.hpp"


class NPCSpawner
{
public:
	NPCSpawner();
	~NPCSpawner();

	void spawn(const std::string& npcType, const sf::Vector2f& position, ActorManager& actorManager, ActorIdTracker& actorIdTracker);
};
