#pragma once

#include "actor/Actor.hpp"
#include "application/Camera.hpp"

const static int TERMINAL_X_WALK = 200;
const static int TERMINAL_X_RUN = 500;
const static int TERMINAL_Y_WALK = 180;
const static int TERMINAL_Y_RUN = 450;

class PlayerActor : public Actor
{
public:
	PlayerActor();
	~PlayerActor();

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool isPlayer() const;
	bool isNPC() const;

	std::weak_ptr<Camera> getCamera() const;

private:
	void control();

	std::shared_ptr<Camera> m_camera;
};

