#include "NoiseHelpers.h"

#include <random>
#include "Math.h"

uint64_t _rng_state = 448778;

float _noise[NOISE_W][NOISE_H];
Vector2i _sampleSize(NOISE_W, NOISE_H);
bool _rngLoaded = false;
std::mt19937_64 _rng;
std::uniform_real_distribution<float> dist01(0, 1); // Create a distribution in range [0 1].
std::uniform_int_distribution<int> dist0255(0, 255); // Create a distribution in range [0 255]
bool _noiseSeedChanged = false;
int _noise2[3 * NOISE_W];

int repeat = -1;
int permutation[] = 
{ 151,160,137,91,90,15,					// Hash lookup table as defined by Ken Perlin.  This is a randomly
	131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,	// arranged array of all numbers from 0-255 inclusive.
	190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
	88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
	77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
	102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
	135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
	5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
	223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
	129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
	251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
	49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
	138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

int p[512];                                                    // Doubled permutation to avoid overflow


float Perlin3D(float x, float y, float z) 
{
	float ab = PerlinNoise2D(x,y);
	float bc = PerlinNoise2D(x, z);
	float ac = PerlinNoise2D(x, z);

	float ba = PerlinNoise2D(y, x);
	float cb = PerlinNoise2D(z, y);
	float ca = PerlinNoise2D(z, x);

	float abc = ab + bc + ac + ba + cb + ca;

	return abc / 6.0f;
}

/// Generate 2D Perlin noise.
float PerlinNoise2D(float x, float y)
{
	return PerlinNoise2D(x, y, SAMPLE_FREQ, SAMPLE_AMP, SAMPLE_PERS, SAMPLE_OCTAVES);
}

float PerlinNoise3D(float x, float y, float z)
{
	return PerlinNoise3D(x, y, z, SAMPLE_FREQ, SAMPLE_AMP, SAMPLE_PERS, SAMPLE_OCTAVES);
}

/// Description: Generates sample noise using a normalised random distribution. Call this function once on initilisation.
void Init_Perlin_Noisy()
{
	Perlin_Init(); // TEST: new noise generator
	// Create a RNG with a random seed.

	if (!_rngLoaded)
	{
		_rng.seed(std::random_device()());

		_rngLoaded = true;
	}

	// Fill the noise sample.
	for (int x = 0; x < _sampleSize.x; x++)
	{
		for (int y = 0; y < _sampleSize.y; y++)
		{
			// Compute normalised noise value.
			_noise[x][y] = Random01();
		}
	}

	// Initilise 3D Noise Permutations
	for (int i = 0; i < 3 * NOISE_W; i++)
	{
		_noise2[i] = dist0255(_rng);
	}

	_noiseSeedChanged = true;
}

float Random01()
{
	//xorshift64();
	//uint64_t r = xorshift64();

	//auto max = std::numeric_limits<uint64_t>::max();
	//float noiseValue = (float)((double)r / (double)max);

	//noiseValue = ClampF(noiseValue, 0, 1);

	//return noiseValue;

	return  dist01(_rng);
	//return (float)rand() / (RAND_MAX);
}

byte RandomByte()
{
	return (byte)(Random01() * 255);
}

uint64_t xorshift64()
{
	uint64_t x = _rng_state;
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	x ^= x >> 5;
	x ^= x << 3;

	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	x ^= x >> 5;
	x ^= x << 3;

	return _rng_state = x;
}

byte* GetNoisePixelData(float freq, float amp, float pers, int octaves, float scaleX, float scaleY, float scaleZ)
{
	byte* _noisePixels = new byte[NOISE_W * NOISE_H * 4]; // RGBA
	int i, x, y;
	float n;
	byte grey;

	// Store noise value in pixel buffer to use as a texture for debugging

	for (x = 0; x < NOISE_W; x++)
	{
		for (y = 0; y < NOISE_H; y++)
		{
			n = OctavePerlin(x * scaleX, y * scaleY, 1 * scaleZ, octaves, pers);
			//n = PerlinNoise2D(x, y, freq, amp, pers, octaves);
			//n = PerlinNoise3D(x, y, 0); //HACK

			i = ((NOISE_H * y) + x);

			grey = n * 255;
			_noisePixels[(i * 4) + 0] = grey;
			_noisePixels[(i * 4) + 1] = grey;
			_noisePixels[(i * 4) + 2] = grey;
			_noisePixels[(i * 4) + 3] = 255;
		}
	}

	_noiseSeedChanged = false;

	return _noisePixels;
}

bool HasNoiseChanged()
{
	return _noiseSeedChanged;
}

float OctavePerlin(float x, float y, float z, int octaves, float persistence)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;            // Used for normalizing result to 0.0 - 1.0
	for (int i = 0; i < octaves; i++)
	{
		total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}

void Perlin_Init()
{
	for (int x = 0; x < 512; x++)
	{
		//p[x] = permutation[x % 256];
		p[x] = Random01() * 255;
	}
}

float perlin(float x, float y, float z)
{
	if (repeat > 0)
	{                                   // If we have any repeat on, change the coordinates to their "local" repetitions
		x = std::fmod(x, repeat);
		y = std::fmod(y, repeat);
		z = std::fmod(z, repeat);
	}

	int xi = (int)x & 255;                              // Calculate the "unit cube" that the point asked will be located in
	int yi = (int)y & 255;                              // The left bound is ( |_x_|,|_y_|,|_z_| ) and the right bound is that
	int zi = (int)z & 255;                              // plus 1.  Next we calculate the location (from 0.0 to 1.0) in that cube.
	float xf = x - (int)x;                             // We also fade the location to smooth the result.
	float yf = y - (int)y;

	float zf = z - (int)z;
	float u = fade(xf);
	float v = fade(yf);
	float w = fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + inc(yi)] + zi];
	aab = p[p[p[xi] + yi] + inc(zi)];
	abb = p[p[p[xi] + inc(yi)] + inc(zi)];
	baa = p[p[p[inc(xi)] + yi] + zi];
	bba = p[p[p[inc(xi)] + inc(yi)] + zi];
	bab = p[p[p[inc(xi)] + yi] + inc(zi)];
	bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

	float x1, x2, y1, y2;
	x1 = lerp(grad(aaa, xf, yf, zf),                // The gradient function calculates the dot product between a pseudorandom
		grad(baa, xf - 1, yf, zf),              // gradient vector and the vector from the input coordinate to the 8
		u);                                     // surrounding points in its unit cube.
	x2 = lerp(grad(aba, xf, yf - 1, zf),                // This is all then lerped together as a sort of weighted average based on the faded (u,v,w)
		grad(bba, xf - 1, yf - 1, zf),              // values we made earlier.
		u);
	y1 = lerp(x1, x2, v);

	x1 = lerp(grad(aab, xf, yf, zf - 1),
		grad(bab, xf - 1, yf, zf - 1),
		u);
	x2 = lerp(grad(abb, xf, yf - 1, zf - 1),
		grad(bbb, xf - 1, yf - 1, zf - 1),
		u);
	y2 = lerp(x1, x2, v);

	return (lerp(y1, y2, w) + 1) / 2;                       // For convenience we bound it to 0 - 1 (theoretical min/max before is -1 - 1)
}

int inc(int num)
{
	num++;
	if (repeat > 0) num %= repeat;

	return num;
}

float grad(int hash, float x, float y, float z)
{
	int h = hash & 15;                                  // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
	float u = h < 8 /* 0b1000 */ ? x : y;              // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.

	float v;                                           // In Ken Perlin's original implementation this was another conditional operator (?:).  I
														// expanded it for readability.

	if (h < 4 /* 0b0100 */)                             // If the first and second significant bits are 0 set v = y
		v = y;
	else if (h == 12 /* 0b1100 */ || h == 14 /* 0b1110*/)// If the first and second significant bits are 1 set v = x
		v = x;
	else                                                // If the first and second significant bits are not equal (0/1, 1/0) set v = z
		v = z;

	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // Use the last 2 bits to decide if u and v are positive or negative.  Then return their addition.
}

float fade(float t)
{
	// Fade function as defined by Ken Perlin.  This eases coordinate values
	// so that they will "ease" towards integral values.  This ends up smoothing
	// the final output.
	return t * t* t* (t * (t * 6 - 15) + 10);         // 6t^5 - 15t^4 + 10t^3
}

float lerp(float a, float b, float x)
{
	return a + x * (b - a);
}

/// Description: Generate 2D Perlin noise.
/// Informal : Computes a 2D noise value given the specified 2D coordinateand settings.
/// Formal : Computes a noise value using the specified settings : octaves, frequency, amplitude, and persistance.
/// Returns : Returns a floating point value between - 1 and 1.
float PerlinNoise2D(float x, float y, float frequency, float amplitude, float persistance, int octaves)
{
	float FinalValue = 0.0f;
	int i;
	float amp = amplitude;
	float freq = frequency;

	//HACK: slow
	float *copy[NOISE_W];
	for (i = 0; i < NOISE_W; i++)
	{
		copy[i] = _noise[i];
	}

	// PERLIN NOISE
	for (i = 0; i < octaves; i++)
	{
		// Smoothing makes more refined perlin noise.
		FinalValue += BilinearInterpolation(x * freq, y * freq, copy) * amp;

		freq *= 2.0f;

		amp *= persistance;
	}

	FinalValue = FinalValue / ((octaves * persistance) * amplitude);

	// Normalise between 0 and 1
	FinalValue = ClampF(0.0f, 1.0f, FinalValue); // normalise between 0 and 1

#ifdef SAMPLE_USE_EXP
											// Apply exponent to reduce artifacts/features in noise.
	FinalValue = std::pow(FinalValue, SAMPLE_ATTENUATION); // feature reduction through attenuation.
#endif

	return FinalValue;
}

float PerlinNoise3D(float x, float y, float z, float frequency, float amplitude, float persistance, int octaves)
{
	float FinalValue = 0.0f;
	int i;
	float amp = amplitude;
	float freq = frequency;

	// PERLIN NOISE
	for (i = 0; i < octaves; i++)
	{
		// Smoothing makes more refined perlin noise.
		FinalValue += Noise(x * freq, y * freq, z * freq) * amp;

		freq *= 2.0f;

		amp *= persistance;
	}

	FinalValue = FinalValue / ((octaves * persistance) * amplitude);

	// Normalise between 0 and 1
	FinalValue = ClampF(0.0f, 1.0f, FinalValue); // normalise between 0 and 1

#ifdef SAMPLE_USE_EXP
											// Apply exponent to reduce artifacts/features in noise.
	FinalValue = std::pow(FinalValue, SAMPLE_ATTENUATION); // feature reduction through attenuation.
#endif

	return FinalValue;
}

/// Description: Bilinear filtering function.
/// Informal : Applies a Bilinear Filter to a 2 Dimensional Array.
/// Formal : Specify a coordinate in the specified sample
/// and the function will return the smoothed value using 4 adjacent values.
/// Could be used in a convolution kernel.
float BilinearInterpolation(float x, float y, float** sample)
{
	float smooth = 0.0f;

	int width = _sampleSize.x;
	int height = _sampleSize.y;

	float fractionX = x - (int)x;
	float fractionY = y - (int)y;

	int X1 = ((int)x + width) % width;
	int Y1 = ((int)y + height) % height;
	int X2 = ((int)x + width - 1) % width;
	int Y2 = ((int)y + height - 1) % height;


	smooth += fractionX * fractionY * sample[X1][Y1];
	smooth += fractionX * (1 - fractionY) * sample[X1][Y2];

	smooth += (1 - fractionX) * fractionY * sample[X2][Y1];
	smooth += (1 - fractionX) * (1 - fractionY) * sample[X2][Y2];

	return smooth;
}

float NoiseWeight(float t)
{
	float t3 = t*t*t;
	float t4 = t3 * t;
	return (6.0f* t4 * t) - (15.0f * t4) + (10.0f * t3);
}

float Grad(int x, int y, int z, float dx, float dy, float dz)
{
	// test permutation table
	int h = _noise2[_noise2[_noise2[x] + y] + z];
	h &= 15;
	float u = (h < 8 || h == 12 || h == 13) ? dx : dy;
	float v = (h < 4 || h == 12 || h == 13) ? dy : dz;
	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}

int FloorI(float val) { return (int)floorf(val); }

float Noise(float x, float y, float z) { return Noise(Vector3(x, y, z)); }

float Noise(Vector3 p)
{
	// Compute noise cell coordinates and offsets
	int ix = FloorI(p.x), iy = FloorI(p.y), iz = FloorI(p.z);
	float dx = p.x - ix, dy = p.y - iy, dz = p.z - iz;

	// Compute gradient weights for each corner of 3D cube (8 verticies)
	ix &= (256 - 1);
	iy &= (256 - 1);
	iz &= (256 - 1);

	float q0 = Grad(ix, iy, iz, dx, dy, dz);
	float q4 = Grad(ix + 1, iy, iz, dx - 1, dy, dz);
	float q2 = Grad(ix, iy + 1, iz, dx, dy - 1, dz);
	float q6 = Grad(ix + 1, iy + 1, iz, dx - 1, dy - 1, dz);
	float q1 = Grad(ix, iy, iz + 1, dx, dy, dz - 1);
	float q5 = Grad(ix + 1, iy, iz + 1, dx - 1, dy, dz - 1);
	float q3 = Grad(ix, iy + 1, iz + 1, dx, dy - 1, dz - 1);
	float q7 = Grad(ix + 1, iy + 1, iz + 1, dx - 1, dy - 1, dz - 1);

	// Smooth 
	return TrilinearInterpolation(p, q0, q1, q2, q3, q4, q5, q6, q7);
}

float TrilinearInterpolation(Vector3 p, float q0, float q1, float q2, float q3, float q4, float q5, float q6, float q7)
{
	int ix = FloorI(p.x), iy = FloorI(p.y), iz = FloorI(p.z);
	float dx = p.x - ix, dy = p.y - iy, dz = p.z - iz;

	// Compute gradient weights for each corner of 3D cube (8 verticies)
	ix &= (256 - 1);
	iy &= (256 - 1);
	iz &= (256 - 1);

	// Compute trilinear interpolation of weights
	float wx = NoiseWeight(dx), wy = NoiseWeight(dy), wz = NoiseWeight(dz);
	float x00 = Coserp(q0, q4, wx);
	float x10 = Coserp(q2, q6, wx);
	float x01 = Coserp(q1, q5, wx);
	float x11 = Coserp(q2, q7, wx);

	float y0 = Coserp(x00, x10, wy);
	float y1 = Coserp(x01, x11, wy);

	return Coserp(y0, y1, wz);
}

float Lerp(float from, float to, float ratio)
{
	return((from * (1 - ratio)) + (to * ratio));
	// return (from + (to - from) * ratio);
}

float Coserp(float from, float to, float ratio)
{
	// Translate first to ratio in a circle  
	float circularRatio = (1 - cos(ratio * PI)) / 2.0f;

	// Interpolate linearly based on circular function
	return Lerp(from, to, circularRatio);
}