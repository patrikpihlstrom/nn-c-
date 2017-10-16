#pragma once

#include "Actor.hpp"


class NNActor : public Actor, public std::enable_shared_from_this<NNActor>
{
public:
	const float SENSOR_DISTANCE = 90.f;
	const unsigned short SENSOR_COUNT = 15.f;

	NNActor();
	~NNActor();

	uint8_t getHealth() const;
	bool isPlayer() const;
	bool isNN() const;

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<sf::Vector2f> getSensors() const;

private:
	void control();

	std::weak_ptr<Actor> m_playerActor;
	std::vector<sf::Vector2f> m_sensors;
	double m_theta;
};

