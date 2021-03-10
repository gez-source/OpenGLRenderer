#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#include <iostream>
#include <string>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.
#include "TextureData.h"
#include "TextureBuilder.h"
#include "OpenGLExt.h"

class TextureData;
class TextureBuilder;

class TextureUtils 
{
public:
	static TextureData* decodeTextureFile(std::string file);
	static GLuint uploadTextureToOpenGL(TextureData* data, TextureBuilder* builder);
	static GLuint uploadTextureToOpenGL(TextureData* dataRight, TextureData* dataLeft,
		TextureData* dataTop, TextureData* dataBottom,
		TextureData* dataBack, TextureData* dataFront, TextureBuilder* builder);
};

#endif