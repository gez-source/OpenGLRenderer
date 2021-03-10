#include "OpenGLExt.h"

/// Get a list of all the extensions available on the current OpenGL context.
std::vector<std::string> GetExtensions()
{
	std::vector<std::string> result;
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++)
	{
		GLubyte* extName = (GLubyte*)glGetStringi(GL_EXTENSIONS, i);

		std::string extension(reinterpret_cast<const char*>(extName));

		result.push_back(extension);
	}
	return result;
}

/// Check if an OpenGL extension is available on the current OpenGL context.
bool HasCapability(std::string extensionName)
{
	bool hasCapability = false;
	GLint n, i;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (i = 0; i < n; i++)
	{
		GLubyte* extName = (GLubyte*)glGetStringi(GL_EXTENSIONS, i);

		std::string extension(reinterpret_cast<const char*>(extName));

		if (extension == extensionName)
		{
			hasCapability = true;
			break;
		}
	}
	return hasCapability;
}