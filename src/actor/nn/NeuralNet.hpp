#pragma once

#include <vector>
#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "Neuron.hpp"


class NeuralNet
{
public:
	NeuralNet();
	NeuralNet(const std::vector<sf::Vector2f> sensors);
	~NeuralNet();

	std::vector<std::shared_ptr<Neuron>> neurons;
	std::vector<std::shared_ptr<Neuron>> output;

	std::vector<float> evaluate(const std::vector<float> input);
};

