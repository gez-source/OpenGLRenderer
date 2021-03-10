#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"
#include "ColourRGBA.h"

class Light
{
public:
	Vector3 position;
	ColourRGBA colour;
	// TODO: type directional, point, spotlight
};

#endif