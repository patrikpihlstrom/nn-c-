#pragma once

#include "Entity.hpp"


class PlayerEntity : public Entity
{
public:
	PlayerEntity();
	~PlayerEntity();

protected:
	void control(const float deltaTime);
};

