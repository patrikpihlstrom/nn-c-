#pragma once

#include <map>
#include <cmath>
#include <random>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>


class NeuralNet
{
	inline static double sigmoidActivationFunction(double x)
	{
		return 1.0/(1.0+std::exp(-x));
	}

	inline static double clampOutputValue(double x)
	{
		return x;
		if (x < 0.1)
		{
			return 0;
		}

		if (x > 0.9)
		{
			return 1;
		}

		return -1;
	}

public:
	enum class ActivationFunction
	{
		Sigmoid,
	};

	struct Settings
	{
		uint32_t numInputs;
		uint32_t numHidden;
		uint32_t numOutputs;
	};

public:
	NeuralNet();
	NeuralNet(Settings const& settings);
	NeuralNet(Settings const& settings, std::vector<double> const& weights);
	~NeuralNet();

	std::vector<double> const& evaluate(std::vector<double> const& input);

	std::vector<double> const& getInputHiddenWeights() const
	{
		return m_weightsInputHidden;
	}

	std::vector<double> const& getHiddenOutputWeights() const
	{
		return m_weightsHiddenOutput;
	}

	std::vector<double> getWeights() const
	{
		std::vector<double> weights = m_weightsInputHidden;
		weights.insert(weights.end(), m_weightsHiddenOutput.begin(), m_weightsHiddenOutput.end());
		return weights;
	}

private:
	void initializeNetwork();
	void initializeWeights();
	void loadWeights(std::vector<double> const& weights);

	int32_t getInputHiddenWeightIndex(int32_t inputIndex, int32_t hiddenIndex) const
	{
		return inputIndex*m_numHidden + hiddenIndex;
	}

	int32_t getHiddenOutputWeightIndex(int32_t hiddenIndex, int32_t outputIndex) const
	{
		return hiddenIndex*m_numOutputs + outputIndex;
	}

private:
	int32_t m_numInputs, m_numHidden, m_numOutputs;

	std::vector<double> m_inputNeurons;
	std::vector<double> m_hiddenNeurons;
	std::vector<double> m_outputNeurons;

	std::vector<double> m_weightsInputHidden; // The weights from the input neurons to the first hidden layer
	std::vector<double> m_weightsHiddenOutput;

	std::vector<double> m_clampedOutputs;
};

