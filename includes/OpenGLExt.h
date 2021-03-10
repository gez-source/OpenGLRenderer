#ifndef OPENGL_EXT_H
#define OPENGL_EXT_H

#include <GL/glew.h>
#include <string>
#include <vector>

std::vector<std::string> GetExtensions();
bool HasCapability(std::string extensionName);

#endif