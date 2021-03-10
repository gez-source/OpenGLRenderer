#include "Cubemap.h"

Cubemap::Cubemap(GLuint textureId) : Texture(textureId, GL_TEXTURE_CUBE_MAP)
{

}

Cubemap::~Cubemap()
{
	Texture::~Texture();
}

TextureBuilder Cubemap::LoadTexture(std::string texFileRightPosX, std::string texFileLeftNegX,
	std::string texFileTopPosY, std::string texFileBottomNegY,
	std::string texFileBackPosZ, std::string texFileFrontNegZ)
{
	return TextureBuilder(texFileRightPosX, texFileLeftNegX, texFileTopPosY, texFileBottomNegY, texFileBackPosZ, texFileFrontNegZ);
}