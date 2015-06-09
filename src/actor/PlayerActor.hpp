#pragma once

#include "actor/Actor.hpp"
#include "application/Camera.hpp"

const static unsigned char TERMINAL_X_WALK = 100;
const static unsigned char TERMINAL_Y_WALK = 75;

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

