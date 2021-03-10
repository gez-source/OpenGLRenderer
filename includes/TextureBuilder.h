#ifndef TEXTURE_BUILDER_H
#define TEXTURE_BUILDER_H

#include <string>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.
#include "Texture.h"
#include "TextureData.h"
#include "TextureUtils.h"

class Texture;
class TextureBuilder;

class TextureBuilder 
{
private:
	bool cubemap = false;
	bool clampEdges = false;
	bool mipmap = false;
	bool anisotropic = true;
	bool nearest = false;

	std::string file;
	std::string texFileRightPosX; 
	std::string texFileLeftNegX;
	std::string texFileTopPosY; 
	std::string texFileBottomNegY;
	std::string texFileBackPosZ; 
	std::string texFileFrontNegZ;

public:
	TextureBuilder(std::string textureFile);
	TextureBuilder(std::string textureFileRightPosX, std::string texFileLeftNegX,
		std::string textureFileTopPosY, std::string texFileBottomNegY,
		std::string textureFileBackPosZ, std::string texFileFrontNegZ);

	Texture* Create();
	TextureBuilder* ClampEdges();
	TextureBuilder* NormalMipMap();
	TextureBuilder* NearestFiltering();
	TextureBuilder* Anisotropic();
	bool isClampEdges() const;
	bool isMipmap() const;
	bool isAnisotropic() const;
	bool isNearest() const;
};

#endif