#include "TextureBuilder.h"
#include "Cubemap.h"

TextureBuilder::TextureBuilder(std::string textureFile)
{
	this->file = textureFile;
}

TextureBuilder::TextureBuilder(std::string texFileRightPosX, std::string texFileLeftNegX,
	std::string texFileTopPosY, std::string texFileBottomNegY,
	std::string texFileBackPosZ, std::string texFileFrontNegZ)
{
	cubemap = true;

	this->texFileRightPosX = texFileRightPosX;
	this->texFileLeftNegX = texFileLeftNegX;
	this->texFileTopPosY = texFileTopPosY;
	this->texFileBottomNegY = texFileBottomNegY;
	this->texFileBackPosZ = texFileBackPosZ;
	this->texFileFrontNegZ = texFileFrontNegZ;
}

Texture* TextureBuilder::Create()
{
	if (cubemap == false)
	{
		TextureData* textureData = TextureUtils::decodeTextureFile(file);

		GLuint textureId = TextureUtils::uploadTextureToOpenGL(textureData, this);

		return new Texture(textureId, textureData);
	}
	else 
	{
		TextureData* dataRight = TextureUtils::decodeTextureFile(texFileRightPosX);
		TextureData* dataLeft = TextureUtils::decodeTextureFile(texFileLeftNegX);
		TextureData* dataTop = TextureUtils::decodeTextureFile(texFileTopPosY);
		TextureData* dataBottom = TextureUtils::decodeTextureFile(texFileBottomNegY);
		TextureData* dataBack = TextureUtils::decodeTextureFile(texFileBackPosZ);
		TextureData* dataFront = TextureUtils::decodeTextureFile(texFileFrontNegZ);

		GLuint textureId = TextureUtils::uploadTextureToOpenGL(dataRight, dataLeft, dataTop, dataBottom, dataBack, dataFront, this);

		return new Cubemap(textureId);
	}
}

TextureBuilder* TextureBuilder::ClampEdges()
{
	this->clampEdges = true;
	return this;
}

TextureBuilder* TextureBuilder::NormalMipMap()
{
	this->mipmap = true;
	this->anisotropic = false;
	return this;
}

TextureBuilder* TextureBuilder::NearestFiltering()
{
	this->mipmap = false;
	this->anisotropic = false;
	this->nearest = true;
	return this;
}

TextureBuilder* TextureBuilder::Anisotropic()
{
	this->mipmap = true;
	this->anisotropic = true;
	return this;
}

bool TextureBuilder::isClampEdges() const
{
	return clampEdges;
}

bool TextureBuilder::isMipmap() const
{
	return mipmap;
}

bool TextureBuilder::isAnisotropic() const
{
	return anisotropic;
}

bool TextureBuilder::isNearest() const
{
	return nearest;
}