#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>


struct Neuron
{
	struct Connection
	{
		float weight;
		Neuron* out;
	};

	float value;
	std::map<unsigned short, std::vector<Connection>> out;
	unsigned short inputs; // the amount of neurons in the previous layer

	Neuron() :
		value(0)
	{
	}

	void fire(const float value = 1)
	{
		this->value = value/inputs;
		for (auto it = out.begin(); it != out.end(); ++it)
		{
			for (int i = 0; i < it->second.size(); ++i)
			{
				auto neuron = it->second[i].out;
				if (neuron)
				{
					neuron->fire(this->value*it->first);
				}
			}
		}
	}
};

