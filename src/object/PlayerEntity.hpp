#pragma once

#include "object/Entity.hpp"


class PlayerEntity : public Entity
{
public:
	PlayerEntity();
	~PlayerEntity();

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	bool isPlayerEntity() const;

private:
	void control(const float& deltaTime);
};

