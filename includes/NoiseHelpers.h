#ifndef NOISE_HELPERS_H
#define NOISE_HELPERS_H

#include <vector>
#include "Vector2i.h"
#include "Vector2.h"
#include "Vector3.h"
#include "MathHelpers.h"
#include "Byte.h"


// Noise settings
#define NOISE_W 256
#define NOISE_H 256
#define SAMPLE_ATTENUATION (1 / 0.7)

// 3D
//#define SAMPLE_FREQ 0.07f
//#define SAMPLE_AMP 1.0f
//#define SAMPLE_PERS 0.1f
//#define SAMPLE_OCTAVES 10

// 2D
#define SAMPLE_FREQ 0.2f
#define SAMPLE_AMP 1.0f
#define SAMPLE_PERS 0.25f
#define SAMPLE_OCTAVES 8

//#define SAMPLE_USE_EXP

// Math constants
//float PI = atan(1) * 4.0f; 
//#define PI 3.14159265358979323846f // 180 degrees
//#define PI2 = (2.0f * PI) // 360 degrees 
//#define PI3OverTwo = ((3.0f * PI) / 2.0f) // 270 degrees
//#define PIOver2 = (PI / 2.0f) // 90 degrees
//#define PIOver4 = (PI / 4.0f) // 45 degrees
//#define MAX_FLOAT 3.402823466e+38F // max floating point value
//#define MIN_FLOAT 1.175494351e-38F // min floating point value

	// NOISE FUNCTIONS
	void Init_Perlin_Noisy(); // perlin noise initilisation function
	float Random01();
	byte RandomByte();
	uint64_t xorshift64();

	float Perlin3D(float x, float y, float z);
	float PerlinNoise2D(float x, float y);
	float PerlinNoise2D(float x, float y, float frequency, float amplitude, float persistance, int octaves);
	float PerlinNoise3D(float x, float y, float z);
	float PerlinNoise3D(float x, float y, float z, float frequency, float amplitude, float persistance, int octaves);
	//byte* GetNoisePixelData();
	byte* GetNoisePixelData(float freq, float amp, float pers, int octaves, float scaleX, float scaleY, float scaleZ);
	bool HasNoiseChanged();

// new perlin noise
	float OctavePerlin(float x, float y, float z, int octaves, float persistence);
	void Perlin_Init();
	float perlin(float x, float y, float z);
	int inc(int num);
	float grad(int hash, float x, float y, float z);
	float fade(float t);
	float lerp(float a, float b, float x);

// new perlin noise

	float Noise(float x, float y, float z);
	float Noise(Vector3 p);

	// INTERPOLATORS
	float Lerp(float from, float to, float ratio); // Linear Interpolator
	float Coserp(float from, float to, float ratio); // Cosine Interpolator
	float BilinearInterpolation(float x, float y, float** sample);
	float TrilinearInterpolation(Vector3 p, float q0, float q1, float q2, float q3, float q4, float q5, float q6, float q7);


//}
#endif
