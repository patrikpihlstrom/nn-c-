#pragma once

#include "Actor.hpp"
#include "nn/NeuralNet.hpp"

#include <algorithm>


class NNActor : public Actor, public std::enable_shared_from_this<NNActor>
{
public:
	const float SENSOR_DISTANCE = 100.f;
	const float SIGHT_DISTANCE = 300.f;
	const uint32_t SENSOR_COUNT = 5;
	const uint32_t EYE_COUNT = 5;

	NNActor();
	NNActor(const std::vector<double> dna);
	~NNActor();

	uint8_t getHealth() const;
	bool isPlayer() const;
	bool isNN() const;

	void update(const float& deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawNeuralNet(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Rect<int> getBounds() const;
	sf::Rect<float> getPhysicalBounds() const;
	std::vector<sf::Vector2f> getSensors() const;

	float getInput(const int index) const;
	void setInput(const float value, const int index);

	bool isDead() const;
	void setDead(const bool dead);

	std::vector<double> getDna() const;
	void printDna() const;

	sf::Vector2f getSensor(const int index);
	void setSensor(const int index, const sf::Vector2f sensor);

	void addHealth(const int health);

private:
	void control();

	std::weak_ptr<Actor> m_playerActor;
	std::vector<sf::Vector2f> m_sensors;
	std::vector<double> m_inputs;
	double m_theta;
	NeuralNet m_neuralNet;
	std::vector<double> m_dna;

	bool m_finished;
};

