#include "NeuralNet.hpp"


NeuralNet::NeuralNet()
{
	initialize();
}

NeuralNet::NeuralNet(const std::vector<sf::Vector2f> sensors) :
	m_hiddenLayers(1)
{
	neurons.insert(std::pair<unsigned short, std::vector<Neuron>>(m_hiddenLayers, std::vector<Neuron>(sensors.size())));
	for (int i = 0; i < sensors.size(); ++i)
	{
		Neuron neuron = {};
		for (int j = 0; j < neurons[1].size(); ++j)
		{
			float weight = (double)std::rand()/RAND_MAX;
			neuron.out[1].push_back({weight, &(neurons[1][j])});
		}

		neurons[m_hiddenLayers].push_back(neuron);
	}

	for (auto it = neurons.rbegin(); it != neurons.rend(); ++it)
	{
		auto previous = std::prev(it);
        for (int i = 0; i < it->second.size(); ++i)
		{
			it->second[i].inputs = previous->second.size();
		}
	}
}

NeuralNet::NeuralNet(const std::map<unsigned short, std::vector<std::vector<float>>> dna) :
    m_hiddenLayers(1)
{
	for (auto layer = dna.begin(); layer != dna.end(); ++layer)
	{
		for (int i = 0; i < layer->second.size(); ++i) // for each neuron in this layer
		{
			if (layer->first + 1 < neurons.size()) // if we're not at the last hidden layer
			{
				neurons[layer->first][i].out[layer->first + 1].clear();
				neurons[layer->first][i].out[layer->first + 1].reserve(neurons[layer->first + 1].size());
				for (int j = 0; j < neurons[layer->first + 1].size(); ++j)
				{
					std::cout << layer->second[i][j] << std::endl;
					//neurons[layer->first][i]->out[layer->first + 1].push_back({layer->second[i][j], neurons[layer->first + 1][j]});
				}
			}
			else
			{
				neurons[layer->first][i].out[layer->first + 1].clear();
				neurons[layer->first][i].out[layer->first + 1].reserve(neurons[layer->first + 1].size());
				for (int j = 0; j < output.size(); ++j)
				{
					std::cout << layer->second[i][j] << std::endl;
					//neurons[layer->first][i]->out[layer->first + 1].push_back({layer->second[i][j], output[j]});
				}
			}
		}
	}

	/*neurons[m_hiddenLayers] = std::vector<std::shared_ptr<Neuron>>();
	for (int i = 0; i < dna.size(); ++i)
	{
		auto neuron = std::shared_ptr<Neuron>(new Neuron());
		for (int j = 0; j < output.size(); ++j)
		{
			neuron->out.insert(std::pair<float, std::shared_ptr<Neuron>>(dna[i][j], output[j]));
		}

		neurons[m_hiddenLayers].push_back(neuron);
	}*/

	for (auto it = neurons.rbegin(); it != neurons.rend(); ++it)
	{
		auto previous = std::prev(it);
        for (int i = 0; i < it->second.size(); ++i)
		{
			it->second[i].inputs = previous->second.size();
		}
	}
}

NeuralNet::~NeuralNet()
{
}

void NeuralNet::initialize()
{
	m_hiddenLayers = 1;
	output.clear();
	neurons.clear();
	output.reserve(2);

	// rotation
	output.push_back(Neuron());

	// speed
	output.push_back(Neuron());

	// input layer
	neurons[0] = std::vector<Neuron>();

	for (int i = m_hiddenLayers; i > 0; --i) // construct hidden layers
	{
		neurons[i] = std::vector<Neuron>();
		for (int n = 0; n < output.size()*2; ++n)
		{
			Neuron neuron = {};
			neuron.out[i+1] = std::vector<Neuron::Connection>();
			if (i == m_hiddenLayers) // connect layer to output layer
			{
				neuron.out[i+1].reserve(output.size());
				for (int j = 0; j < output.size(); ++j)
				{
					float weight = (double)std::rand()/RAND_MAX;
					std::cout << weight << std::endl;
					neuron.out[i+1].push_back({weight, &output[j]});
				}
			}
			else // connect layer to previous layer
			{
				neuron.out[i+1].reserve(neurons[i + 1].size());
				for (int j = 0; j < neurons[i + 1].size(); ++j)
				{
					float weight = (double)std::rand()/RAND_MAX;
					std::cout << weight << std::endl;
					neuron.out[i+1].push_back({weight, &neurons[i + 1][j]});
					neurons[i + 1][j].inputs = neurons[i].size();
				}
			}

			neurons[i].push_back(neuron);
		}
	}
}

std::map<float, int> NeuralNet::evaluate(const std::vector<float> input)
{
	std::map<float, int> result;

	for (int i = 0; i < neurons[0].size(); ++i)
	{
		if (i < input.size() && input[i] > 0.f)
		{
			neurons[0][i].fire(input[i]);
		}
	}

	for (int i = 0; i < output.size(); ++i)
	{
		result.insert(std::pair<float, int>(output[i].value, i));
		output[i].value = 0;
	}

	return result;
}

std::map<unsigned short, std::vector<std::vector<float>>> NeuralNet::getDna()
{
	std::map<unsigned short, std::vector<std::vector<float>>> dna;
	for (auto it = neurons.begin(); it != neurons.end(); ++it) // for each layer
	{
		dna[it->first] = std::vector<std::vector<float>>(it->second.size());
		for (int i = 0; i < it->second.size(); ++i) // for each neuron in layer
		{
			dna[it->first][i].reserve(it->second[i].out[it->first].size());
			for (int j = 0; j < it->second[i].out[it->first].size(); ++j) // for each connection in neuron
			{
				dna[it->first][i].push_back(it->second[i].out[it->first][j].weight);
			}
		}
	}

	for (auto it = dna.begin(); it != dna.end(); ++it)
	{
		std::cout << "[";
		for (int i = 0; i < it->second.size(); ++i)
		{
			std::cout << "[";
			for (int j = 0; j < it->second[i].size(); ++j)
			{
				std::cout << it->second[i][j];

				if (j < it->second[i].size() - 1)
				{
					std::cout << ',';
				}
			}
			std::cout << "]";
		}
		std::cout << "]" << std::endl;
	}

	return dna;
}

