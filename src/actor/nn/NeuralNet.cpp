#include "NeuralNet.hpp"

#include <assert.h>


NeuralNet::NeuralNet()
{
}

NeuralNet::NeuralNet(NeuralNet::Settings const& settings) :
	m_numInputs(settings.numInputs),
	m_numHidden(settings.numHidden),
	m_numOutputs(settings.numOutputs)
{
	assert(settings.numInputs > 0 && settings.numOutputs > 0 && settings.numHidden > 0);

	initializeNetwork();
	initializeWeights();
}

NeuralNet::NeuralNet(Settings const& settings, std::vector<double> const& weights) :
	m_numInputs(settings.numInputs),
	m_numHidden(settings.numHidden),
	m_numOutputs(settings.numOutputs)
{
	assert(settings.numInputs > 0 && settings.numOutputs > 0 && settings.numHidden > 0);

	initializeNetwork();
	loadWeights(weights);
}

NeuralNet::~NeuralNet()
{
}

void NeuralNet::initializeNetwork()
{
	// bias neurons for the input and hidden layers
	int32_t const totalNumInputs = m_numInputs + 1;
	int32_t const totalNumHiddens = m_numHidden + 1;

	m_inputNeurons.resize(totalNumInputs);
	m_hiddenNeurons.resize(totalNumHiddens);
	m_outputNeurons.resize(m_numOutputs);
	m_clampedOutputs.resize(m_numOutputs);

	memset(m_inputNeurons.data(), 0, m_inputNeurons.size() * sizeof(double));
	memset(m_hiddenNeurons.data(), 0, m_hiddenNeurons.size() * sizeof(double));
	memset(m_outputNeurons.data(), 0, m_outputNeurons.size() * sizeof(double));
	memset(m_clampedOutputs.data(), 0, m_clampedOutputs.size() * sizeof(int32_t));

	// set the bias values
	m_inputNeurons.back() = -1.0;
	m_hiddenNeurons.back() = -1.0;

	// init layer weights
	int32_t const numInputHiddenWeights = totalNumInputs * totalNumHiddens;
	int32_t const numHiddenOutputWeights = totalNumHiddens * m_numOutputs;
	m_weightsInputHidden.resize(numInputHiddenWeights);
	m_weightsHiddenOutput.resize(numHiddenOutputWeights);
}

void NeuralNet::initializeWeights()
{
	std::random_device rd;
	std::mt19937 generator(rd());

	double const distributionRangeHalfWidth = (2.4/m_numInputs);
	double const standardDeviation = distributionRangeHalfWidth * 2/6;
	std::normal_distribution<> normalDistribution(0, standardDeviation);

	// set weights to normally distributed random values between [-2.4 / numInputs, 2.4 / numInputs]
	for (int32_t inputIndex = 0; inputIndex <= m_numInputs; inputIndex++)
	{
		for (int32_t hiddenIndex = 0; hiddenIndex < m_numHidden; hiddenIndex++)
		{
			int32_t const weightIndex = getInputHiddenWeightIndex(inputIndex, hiddenIndex);
			double const weight = normalDistribution(generator);
			m_weightsInputHidden[weightIndex] = weight;
		}
	}

	// set weights to normally distributed random values between [-2.4 / numInputs, 2.4 / numInputs]
	for (int32_t hiddenIndex = 0; hiddenIndex <= m_numHidden; hiddenIndex++)
	{
		for (int32_t outputIndex = 0; outputIndex < m_numOutputs; outputIndex++)
		{
			int32_t const weightIndex = getHiddenOutputWeightIndex(hiddenIndex, outputIndex);
			double const weight = normalDistribution(generator);
			m_weightsHiddenOutput[weightIndex] = weight;
		}
	}
}

void NeuralNet::loadWeights(std::vector<double> const& weights)
{
	int32_t const numInputHiddenWeights = m_numInputs * m_numHidden;
	int32_t const numHiddenOutputWeights = m_numHidden * m_numOutputs;
	assert(weights.size() == numInputHiddenWeights + numHiddenOutputWeights);

	int32_t weightIndex = 0;
	for (auto inputHiddenIndex = 0; inputHiddenIndex < numInputHiddenWeights; inputHiddenIndex++)
	{
		m_weightsInputHidden[inputHiddenIndex] = weights[weightIndex];
		weightIndex++;
	}

	for (auto hiddenOutputIndex = 0; hiddenOutputIndex < numHiddenOutputWeights; hiddenOutputIndex++)
	{
		m_weightsHiddenOutput[hiddenOutputIndex] = weights[weightIndex];
		weightIndex++;
	}
}

std::vector<double> const& NeuralNet::evaluate(std::vector<double> const& input)
{
	/*for (int i = 0; i < input.size(); ++i)
	{
		std::cout << i << ": " << input[i] << std::endl;
	}*/

	assert(input.size() == m_numInputs);
	assert(m_inputNeurons.back() == -1.0 && m_hiddenNeurons.back() == -1.0);

	// Set input values
	memcpy(m_inputNeurons.data(), input.data(), input.size() * sizeof(double));

	// Update hidden neurons
	for (int32_t hiddenIndex = 0; hiddenIndex < m_numHidden; hiddenIndex++)
	{
		m_hiddenNeurons[hiddenIndex] = 0;
		for (int32_t inputIndex = 0; inputIndex <= m_numInputs; inputIndex++)
		{
			int32_t const weightIndex = getInputHiddenWeightIndex(inputIndex, hiddenIndex);
			m_hiddenNeurons[hiddenIndex] += m_inputNeurons[inputIndex] * m_weightsInputHidden[weightIndex];
		}

		m_hiddenNeurons[hiddenIndex] = sigmoidActivationFunction(m_hiddenNeurons[hiddenIndex]);
	}

	for (int32_t outputIndex = 0; outputIndex < m_numOutputs; outputIndex++)
	{
		m_outputNeurons[outputIndex] = 0;

		for (int32_t hiddenIndex = 0; hiddenIndex <= m_numHidden; hiddenIndex++)
		{
			int32_t const weightIndex = getHiddenOutputWeightIndex(hiddenIndex, outputIndex);
			m_outputNeurons[outputIndex] += m_hiddenNeurons[hiddenIndex] * m_weightsHiddenOutput[weightIndex];
		}

		m_outputNeurons[outputIndex] = sigmoidActivationFunction(m_outputNeurons[outputIndex]);
		m_clampedOutputs[outputIndex] = clampOutputValue(m_outputNeurons[outputIndex]);
	}

	std::sort(m_clampedOutputs.begin(), m_clampedOutputs.end());
	return m_clampedOutputs;
}

