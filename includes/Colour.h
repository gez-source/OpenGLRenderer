#ifndef COLOUR_H
#define COLOUR_H

#include "Byte.h"
#include "ColourRGBA.h"
#include "Vector3.h"

//namespace ath
//{
	class ColourRGBA;
	class Vector3;

	class Colour
	{
	public:

		static ColourRGBA red;
		static ColourRGBA green;
		static ColourRGBA blue;
		static ColourRGBA cyan;
		static ColourRGBA black;
		static ColourRGBA yellow;
		static ColourRGBA magenta;
		static ColourRGBA gray;
		static ColourRGBA white;
		static ColourRGBA clear;
		static ColourRGBA grey;

		static ColourRGBA Randomise();

		static Vector3 ToHSV(ColourRGBA colour);

		static ColourRGBA FromHSV(Vector3 hsvColour);

		static ColourRGBA FromHSV(float hue, byte saturation, byte value);

		static ColourRGBA FromHSV(float hue, float saturation, float value);
	};

//}
#endif
