#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>


struct NPCSpawnEvent
{
	std::string type;
	sf::Vector2i position;
};

