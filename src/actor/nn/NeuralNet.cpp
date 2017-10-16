#include "NeuralNet.hpp"


NeuralNet::NeuralNet()
{
	output.reserve(4);
	// left
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// right
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// + speed
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// - speed
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));
}

NeuralNet::NeuralNet(const std::vector<sf::Vector2f> sensors)
{
	output.reserve(4);
	// left
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// right
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// + speed
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	// - speed
	output.push_back(std::shared_ptr<Neuron>(new Neuron()));

	for (int i = 0; i < sensors.size(); ++i)
	{
		auto neuron = std::shared_ptr<Neuron>(new Neuron());
		for (int j = 0; j < output.size(); ++j)
		{
			neuron->out.insert(std::pair<float, std::shared_ptr<Neuron>>(1.f, output[j]));
		}

		neurons.push_back(neuron);
	}
}

NeuralNet::~NeuralNet()
{
}

std::vector<float> NeuralNet::evaluate(const std::vector<float> input)
{
	std::vector<float> result;
	result.resize(output.size(), 0);

	for (int i = 0; i < neurons.size(); ++i)
	{
		if (i < input.size())
		{
			neurons[i]->fire(input[i]);
		}
	}

	for (int i = 0; i < output.size(); ++i)
	{
		result[i] = output[i]->value/input.size();
		output[i]->value = 0;
	}

	return result;
}

