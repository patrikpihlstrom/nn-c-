#pragma once

#include "Actor.hpp"
#include "nn/NeuralNet.hpp"

#include <algorithm>


class NNActor : public Actor, public std::enable_shared_from_this<NNActor>
{
public:
	const float SENSOR_DISTANCE = 180.f;
	const unsigned short SENSOR_COUNT = 3;

	NNActor();
	NNActor(const std::map<unsigned short, std::vector<std::vector<float>>> dna);
	~NNActor();

	uint8_t getHealth() const;
	bool isPlayer() const;
	bool isNN() const;

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Rect<int> getBounds() const;
	sf::Rect<float> getPhysicalBounds() const;
	std::vector<sf::Vector2f> getSensors() const;

	float getInput(const int index) const;
	void setInput(const float value, const int index);

	bool isDead() const;
	void setDead(const bool dead);

	std::map<unsigned short, std::vector<std::vector<float>>> getDna() const;
	void printDna() const;

private:
	void control();

	std::weak_ptr<Actor> m_playerActor;
	std::vector<sf::Vector2f> m_sensors;
	std::vector<float> m_inputs;
	double m_theta;
	NeuralNet m_neuralNet;
	std::map<unsigned short, std::vector<std::vector<float>>> m_dna;
};

