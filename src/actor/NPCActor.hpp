#pragma once

#include "actor/Actor.hpp"


class NPCActor : public Actor
{
public:
	NPCActor();
	~NPCActor();

	bool isPlayer() const;
	bool isNPC() const;

private:
	void control();
	sf::Vector2f m_target;

	bool m_grounded;
};

