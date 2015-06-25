#pragma once


class Noise
{
public:
	Noise();
	~Noise();

	long getSeed() const;
	void setSeed(const long& seed);

	template <typename T>
	T getVal(const T& x, const T& y) const;

private:
	long m_seed;
};

