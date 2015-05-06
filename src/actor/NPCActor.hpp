#pragma once

#include "actor/Actor.hpp"


class NPCActor : public Actor
{
public:
	NPCActor();
	~NPCActor();

	bool isPlayer() const;
	bool isNPC() const;
};

