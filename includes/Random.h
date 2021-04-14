#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>

class Random
{
public:
	uint32_t seed = 0;

	bool RandomBool();
	int RandomInt(int min, int max);
	double RandomDouble(double min, double max);
	double RandomFloat(float min, float max);
	void Seed(uint32_t seed);
	uint32_t Rand();
	uint32_t Rand_Lehmer32();
};

#endif