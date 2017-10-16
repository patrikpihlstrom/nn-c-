#pragma once

#include <map>
#include <memory>


struct Neuron
{
	float value;
	std::map<float, std::weak_ptr<Neuron>> out;

	void fire(const float value = 1)
	{
		this->value = value;
		for (auto it = out.begin(); it != out.end(); ++it)
		{
			auto neuron = it->second.lock();
			if (neuron)
			{
				neuron->fire(it->first*value);
			}
		}
	}
};

