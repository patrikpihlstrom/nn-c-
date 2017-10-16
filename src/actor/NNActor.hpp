#pragma once

#include "Actor.hpp"
#include "nn/NeuralNet.hpp"


class NNActor : public Actor, public std::enable_shared_from_this<NNActor>
{
public:
	const float SENSOR_DISTANCE = 90.f;
	const unsigned short SENSOR_COUNT = 15;

	NNActor();
	~NNActor();

	uint8_t getHealth() const;
	bool isPlayer() const;
	bool isNN() const;

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Rect<int> getBounds() const;
	std::vector<sf::Vector2f> getSensors() const;

	float getInput(const int index) const;
	void setInput(const float value, const int index);

private:
	void control();

	std::weak_ptr<Actor> m_playerActor;
	std::vector<sf::Vector2f> m_sensors;
	std::vector<float> m_inputs;
	double m_theta;
	NeuralNet m_neuralNet;
};

