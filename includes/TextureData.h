#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include "Vector2i.h"
#include "Byte.h"
#include "TGADecoder.h"

class TextureData 
{
private:
	int width;
	int height;
	int bytesPerPixel;
	byte* buffer;

public:
	TextureData(byte* buffer, int width, int height);
	TextureData(byte* buffer, int width, int height, int bytesPerPixel);
	~TextureData();
	int getWidth();
	int getHeight();
	Vector2i getSize();
	byte* getBuffer();
	ColourRGBA get(int x, int y);
	ColourRGBA get(float u, float v);
	int getBytesPerPixel();
	Vector3 normal(Vector2 uvf);
};

#endif