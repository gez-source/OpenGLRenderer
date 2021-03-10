#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.
#include "TextureBuilder.h"
#include "TextureData.h"

class TextureBuilder;

class Texture
{
protected:
	GLuint type;
public:
	GLuint textureId;
	TextureData* textureData;

	Texture();
	Texture(GLuint textureId, GLuint type);
	Texture(GLuint textureId, TextureData* textureData);
	Texture(GLuint textureId, GLuint type, TextureData* textureData);
	~Texture();

	int getWidth();
	int getHeight();
	void bindToUnit(GLuint unit);

	static TextureBuilder LoadTexture(std::string textureFile);
};

#endif