#include "Random.h"

Random::Random(unsigned int seed) : _rng(seed) {}

int Random::uniformInt(int a, int b) {
	std::uniform_int_distribution<int> dist(a, b);
	return dist(_rng);
}

double Random::uniform01() {
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(_rng);
}

bool Random::chancePerSecond(double ratePerSec, double dtSeconds) {
	const double p = ratePerSec * dtSeconds;
	return uniform01() < p;
}