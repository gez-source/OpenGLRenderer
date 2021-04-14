#include "Random.h"

bool Random::RandomBool()
{
	return Rand() % 2 == 1;
}

int Random::RandomInt(int min, int max)
{
	return min + (Rand() % (max - min));
}

double Random::RandomDouble(double min, double max)
{
	double r = (double)Rand();
	double div = (double)(0xFFFFFFFF);
	double scalar = min + (max - min);
	return scalar * (r / div);
}

double Random::RandomFloat(float min, float max)
{
	double r = (double)Rand();
	double div = (double)(0xFFFFFFFF);
	double scalar = (double)min + ((double)max - (double)min);
	return (float)(scalar * (r / div));
}

void Random::Seed(uint32_t seed)
{
	this->seed = seed;
}

uint32_t Random::Rand()
{
	return Rand_Lehmer32();
}

// Fast random number generator, good for procedural generation.
uint32_t Random::Rand_Lehmer32()
{	
	// Lehmer pseudo random number generator.
	uint64_t tmp;
	uint32_t m1;
	uint32_t m2;

	seed += 0xe120fc15;
	
	tmp = (uint64_t)seed * 0x4a39b70d;
	m1 = (tmp >> 32) ^ tmp;

	tmp = (uint64_t)m1 * 0x12fad5c9;
	m2 = (tmp >> 32) ^ tmp;
	
	return m2;

	// Modified from this for 64-bit systems:
	// https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
}