#include "world/procedural/Noise.hpp"


Noise::Noise()
{
}

Noise::~Noise()
{
}

int Noise::getSeed() const
{
	return m_seed;
}

void Noise::setSeed(const int seed)
{
	m_seed = seed;
}

double Noise::getPersistence() const
{
	return m_persistence;
}

void Noise::setPersistence(const double persistence)
{
	m_persistence = persistence;
}

double Noise::getFrequenct() const
{
	return m_frequency;
}

void Noise::setFrequency(const double frequency)
{
	m_frequency = frequency;
}

double Noise::getAmplitude() const
{
	return m_amplitude;
}

void Noise::setAmplitude(const double amplitude)
{
	m_amplitude = amplitude;
}

int Noise::getOctaves() const
{
	return m_octaves;
}

void Noise::setOctaves(const int octaves)
{
	m_octaves = octaves;
}

double Noise::getHeight(const double x, const double y) const
{
	return m_amplitude*total(x, y);
}

double Noise::total(const double i, const double j) const
{
	double t = 0.0f;
	double amplitude = 1;
	double frequency = m_frequency;

	for (int k = 0; k < m_octaves; ++k)
	{
		t += getValue(j*frequency + m_seed, i*frequency + m_seed)*amplitude;
		amplitude *= m_persistence;
		frequency *= 2;
	}

	return t;
}

double Noise::getValue(const double x, const double y) const
{
	int Xint = (int)x, Yint = (int)y;
	double Xfrac = x - Xint, Yfrac = y - Yint;

	double n01 = noise(Xint-1, Yint-1);
	double n02 = noise(Xint+1, Yint-1);
	double n03 = noise(Xint-1, Yint+1);
	double n04 = noise(Xint+1, Yint+1);
	double n05 = noise(Xint-1, Yint);
	double n06 = noise(Xint+1, Yint);
	double n07 = noise(Xint, Yint-1);
	double n08 = noise(Xint, Yint+1);
	double n09 = noise(Xint, Yint);

	double n12 = noise(Xint+2, Yint-1);
	double n14 = noise(Xint+2, Yint+1);
	double n16 = noise(Xint+2, Yint);

	double n23 = noise(Xint-1, Yint+2);
	double n24 = noise(Xint+1, Yint+2);
	double n28 = noise(Xint, Yint+2);

	double n34 = noise(Xint+2, Yint+2);

	double x0y0 = 0.0625*(n01+n02+n03+n04) + 0.125*(n05+n06+n07+n08) + 0.25*(n09);
	double x1y0 = 0.0625*(n07+n12+n08+n14) + 0.125*(n09+n16+n02+n04) + 0.25*(n06);
	double x0y1 = 0.0625*(n05+n06+n23+n24) + 0.125*(n03+n04+n09+n28) + 0.25*(n08);
	double x1y1 = 0.0625*(n09+n16+n28+n34) + 0.125*(n08+n14+n06+n24) + 0.25*(n04);

	double v1 = interpolate(x0y0, x1y0, Xfrac);
	double v2 = interpolate(x0y1, x1y1, Xfrac);
	double fin = interpolate(v1, v2, Yfrac);

	return fin;
}

double Noise::interpolate(const double x, const double y, const double a) const
{
	double negA = 1.0 - a;
	double negASqr = negA*negA;
	double fac1 = 3.0*(negASqr) - 2.0*(negASqr*negA);
	double aSqr = a*a;
	double fac2 = 3.0*aSqr - 2.0*(aSqr*a);

	return x*fac1 + y*fac2;
}

double Noise::noise(const int x,const int y) const
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	int t = (n*(n*n*15731+789221) + 1376312589) & 0x7fffffff;
	return 1.0 - double(t)*0.931322574615478515625e-9;
}

