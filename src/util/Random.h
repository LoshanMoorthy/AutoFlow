#pragma once
#include <random>

class Random {
public:
	explicit Random(unsigned int seed);

	int uniformInt(int a, int b);
	double uniform01();

	// approx Poisson: p ~= rate * dt for small dt
	bool chancePerSecond(double ratePerSec, double dtSeconds);

private:
	std::mt19937 _rng;
};