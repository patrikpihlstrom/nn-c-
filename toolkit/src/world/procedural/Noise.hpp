#pragma once

#include <vector>
#include <algorithm>


class Noise
{
public:
	Noise();
	~Noise();

	double getHeight(const double x, const double y) const;

	int getSeed() const;
	void setSeed(const int seed);

	double getPersistence() const;
	void setPersistence(const double persistence);

	double getFrequenct() const;
	void setFrequency(const double frequency);

	double getAmplitude() const;
	void setAmplitude(const double amplitude);

	int getOctaves() const;
	void setOctaves(const int octaves);

private:
	int m_seed;
	double m_persistence;
	double m_frequency;
	double m_amplitude;
	int m_octaves;

	double total(const double i, const double j) const;
	double getValue(const double x, const double y) const;
	double interpolate(const double x, const double y, const double a) const;
	double noise(const int x, const int y) const;
};

