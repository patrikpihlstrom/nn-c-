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
			float weight = (double)std::rand()/RAND_MAX;
			std::cout << weight << std::endl;
			neuron->out.insert(std::pair<float, std::shared_ptr<Neuron>>(weight, output[j]));
		}

		neurons.push_back(neuron);
	}
}

NeuralNet::~NeuralNet()
{
}

std::map<float, int> NeuralNet::evaluate(const std::vector<float> input)
{
	std::map<float, int> result;

	for (int i = 0; i < neurons.size(); ++i)
	{
		if (i < input.size() && input[i] > 0.f)
		{
			neurons[i]->fire(input[i]);
		}
	}

	for (int i = 0; i < output.size(); ++i)
	{
		result.insert(std::pair<float, int>(output[i]->value/input.size(), i));
		output[i]->value = 0;
	}

	return result;
}

