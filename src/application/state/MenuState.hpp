#pragma once

#include "application/state/State.hpp"


class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void enter();
	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

