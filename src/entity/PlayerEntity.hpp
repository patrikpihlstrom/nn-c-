#pragma once

#include "Entity.hpp"
#include "JumpCheck.hpp"


class PlayerEntity : public Entity
{
public:
	PlayerEntity();
	~PlayerEntity();

protected:
	void control();
	void entitySpecificMovement(const sf::Vector2f& offset);
	void checkCollisions();
	void terminalVelocity();

private:
	JumpCheck m_jumpChecks[3];
};

