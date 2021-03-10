#include "TextureData.h"

TextureData::TextureData(byte* buffer, int width, int height)
{
	this->buffer = buffer;
	this->width = width;
	this->height = height;
	this->bytesPerPixel = 3;
}

TextureData::TextureData(byte* buffer, int width, int height, int bytesPerPixel)
{
	this->buffer = buffer;
	this->width = width;
	this->height = height;
	this->bytesPerPixel = bytesPerPixel;
}

TextureData::~TextureData()
{
	if (this->buffer != nullptr)
	{
		delete this->buffer;

		this->buffer = nullptr;
	}
}

int TextureData::getWidth()
{
	return width;
}

int TextureData::getHeight()
{
	return height;
}

Vector2i TextureData::getSize()
{
	return Vector2i(width, height);
}

byte* TextureData::getBuffer()
{
	return buffer;
}

ColourRGBA TextureData::get(int x, int y)
{
	if (!buffer || x < 0 || y < 0 || x >= width || y >= height) {
		return ColourRGBA();
	}

	byte* pCol = buffer + (x + y * width) * bytesPerPixel; // brga

	return ColourRGBA(pCol[3], pCol[1], pCol[2], pCol[0]);
}

ColourRGBA TextureData::get(float u, float v)
{
	int x = width * u; 
	int y = height * v;

	if (!buffer || x < 0 || y < 0 || x >= width || y >= height) {
		return ColourRGBA();
	}

	byte* pCol = buffer + (x + y * width) * bytesPerPixel; // brga

	return ColourRGBA(pCol[3], pCol[1], pCol[2], pCol[0]);
}

int TextureData::getBytesPerPixel()
{
	return bytesPerPixel;
}

Vector3 TextureData::normal(Vector2 uvf)
{
	ColourRGBA c = get(uvf.x, uvf.y);
	Vector3 normal;

	normal.z = c.b * 2.f - 1.f;
	normal.y = c.r * 2.f - 1.f;
	normal.x = c.g * 2.f - 1.f;

	return normal;
}