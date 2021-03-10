#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include "TextureBuilder.h"

class Cubemap : public Texture
{	
public:
	Cubemap(GLuint textureId);
	~Cubemap();

	static TextureBuilder LoadTexture(std::string texFileRightPosX, std::string texFileLeftNegX, 
									   std::string texFileTopPosY, std::string texFileBottomNegY, 
									   std::string texFileBackPosZ, std::string texFileFrontNegZ);
};

#endif