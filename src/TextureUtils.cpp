#include "TextureUtils.h"
#include "TGADecoder.h"
#include <vector>

TextureData* TextureUtils::decodeTextureFile(std::string file)
{
	int width = 0;
	int height = 0;
	int bytesPerPixel = 0;
	byte* buffer = nullptr;

	TGADecoder* tgaDecoder = new TGADecoder();

	bool loaded = tgaDecoder->read_tga_file(file);
	tgaDecoder->flip_vertically();
	width = tgaDecoder->get_width();
	height = tgaDecoder->get_height();
	bytesPerPixel = tgaDecoder->get_bytesPerPixel();
	buffer = tgaDecoder->buffer(); // in BRGA format

	std::cout << "Texture file " << file << " loading " << (loaded ? "ok" : "failed") << std::endl;

	//TODO: handle PNG decoding

	//TODO: handle DXT and mipmaps http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

	return new TextureData(buffer, width, height, bytesPerPixel);
}

GLuint TextureUtils::uploadTextureToOpenGL(TextureData* data, TextureBuilder* builder)
{
	GLuint textureId;
	GLenum format;
	GLint internalFormat;

	glGenTextures(1, &textureId);
	
	if (data->getBuffer())
	{
		glBindTexture(GL_TEXTURE_2D, textureId);

		//TODO: handle PNG input format
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		
		// PNG Stream Format
		//internalFormat = GL_RGBA; // PNG
		//format = GL_BGRA; // PNG

		// TGA Stream Format
		if(data->getBytesPerPixel() == 4)
		{
			internalFormat = GL_RGBA8;
			format = GL_BGRA;
		}
		else // bytes per pixel = 3
		{
			internalFormat = GL_RGB8;
			format = GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, data->getWidth(), data->getHeight(), 0, format, GL_UNSIGNED_BYTE, data->getBuffer());	

		if (builder->isMipmap())
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (builder->isAnisotropic() && HasCapability("GL_EXT_texture_filter_anisotropic"))
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
			}
		}
		else if (builder->isNearest())
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		if (builder->isClampEdges())
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	return textureId;
}

GLuint TextureUtils::uploadTextureToOpenGL(TextureData* dataRight, TextureData* dataLeft,
	TextureData* dataTop, TextureData* dataBottom,
	TextureData* dataBack, TextureData* dataFront, TextureBuilder* builder)
{
	GLuint textureId;
	GLenum format;
	GLint internalFormat;

	glGenTextures(1, &textureId);

	std::vector<TextureData*> data;
	data.push_back(dataRight);
	data.push_back(dataLeft);
	data.push_back(dataTop);
	data.push_back(dataBottom);
	data.push_back(dataBack);
	data.push_back(dataFront);

	if (dataRight->getBuffer() != nullptr && dataLeft->getBuffer() != nullptr 
		&& dataTop->getBuffer() != nullptr && dataBottom->getBuffer() != nullptr 
		&& dataBack->getBuffer() != nullptr && dataFront->getBuffer() != nullptr)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		//TODO: handle PNG input format
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (GLuint i = 0; i < data.size(); i++)
		{
			// PNG Stream Format
			//internalFormat = GL_RGBA; // PNG
			//format = GL_BGRA; // PNG

			// TGA Stream Format
			if (data[i]->getBytesPerPixel() == 4)
			{
				internalFormat = GL_RGBA8;
				format = GL_BGRA;
			}
			else // bytes per pixel = 3
			{
				internalFormat = GL_RGB8;
				format = GL_BGR;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, data[i]->getWidth(), data[i]->getHeight(), 0, format, GL_UNSIGNED_BYTE, data[i]->getBuffer());
		}

		if (builder->isMipmap())
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (builder->isAnisotropic() && HasCapability("GL_EXT_texture_filter_anisotropic"))
			{
				glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
			}
		}
		else if (builder->isNearest())
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		if (builder->isClampEdges())
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	return textureId;
}