#pragma once

#include <map>
#include <vector>
#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "Neuron.hpp"


class NeuralNet
{
public:
	NeuralNet();
	NeuralNet(const std::vector<sf::Vector2f> sensors);
	NeuralNet(const std::map<unsigned short, std::vector<std::vector<float>>> dna);
	~NeuralNet();

	std::map<unsigned short, std::vector<Neuron>> neurons;
	std::vector<Neuron> output;

	std::map<float, int> evaluate(const std::vector<float> input);

	std::map<unsigned short, std::vector<std::vector<float>>> getDna();

private:
	void initialize();

	unsigned short m_hiddenLayers; 
};

