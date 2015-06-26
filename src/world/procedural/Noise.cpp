#include "world/procedural/Noise.hpp"


Noise::Noise()
{
}

Noise::~Noise()
{
}

long Noise::getSeed() const
{
	return m_seed;
}

void Noise::setSeed(const long& seed)
{
	m_seed = seed;
}

template <typename T>
T Noise::getVal(const T& x, const T& y) const
{
	return T();
}

