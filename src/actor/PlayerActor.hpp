#pragma once

#include "actor/Actor.hpp"
#include "application/Camera.hpp"


class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	bool isPlayer() const;
	bool isNPC() const;

private:
	void control();
};

