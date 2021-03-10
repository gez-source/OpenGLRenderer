#ifndef SHADER_HELPERS_H
#define SHADER_HELPERS_H

#include <iostream>
#include <map>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include "IOHelpers.h"
#include "ShaderProgram.h"

void printShaderLog(GLuint shader);
void printProgramLog(GLuint program);

GLuint loadShader(std::string file, GLenum type);
GLuint loadShader(std::string fileNameVertexShader, std::string fileNameFragShader);
GLuint loadShaderFS(std::string fileNameFragShader);

#endif