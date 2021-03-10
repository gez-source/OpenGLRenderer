#include "Texture.h"

Texture::Texture() : textureId(0), type(0), textureData(nullptr)
{

}

Texture::Texture(GLuint textureId, GLuint type) 
{
	this->textureId = textureId;
	this->type = type;
	this->textureData = nullptr;
}

Texture::Texture(GLuint textureId, TextureData* textureData)
{
	this->textureId = textureId;
	this->type = GL_TEXTURE_2D;
	this->textureData = textureData;
}

Texture::Texture(GLuint textureId, GLuint type, TextureData* textureData)
{
	this->textureId = textureId;
	this->type = type;
	this->textureData = textureData;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureId);

	if (textureData != nullptr)
	{
		delete textureData;
	}
}

int Texture::getWidth()
{
	return textureData->getWidth();
}

int Texture::getHeight()
{
	return textureData->getHeight();
}

void Texture::bindToUnit(GLuint unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, textureId);
}

TextureBuilder Texture::LoadTexture(std::string textureFile)
{
	return TextureBuilder(textureFile);
}