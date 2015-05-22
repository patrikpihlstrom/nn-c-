#pragma once

#include <string>

#include "actor/NPCActor.hpp"


class NPCSpawner
{
public:
	NPCSpawner();
	~NPCSpawner();

private:
	void spawn(const std::string& npcType, const sf::Vector2f& position);
};

