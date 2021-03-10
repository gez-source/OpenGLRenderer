#ifndef UNIFORM_MATRIX_H
#define UNIFORM_MATRIX_H

#include "Uniform.h"
#include "Matrix4.h"

class UniformMatrix : public Uniform 
{
public:
	UniformMatrix()
	{
		
	}

	UniformMatrix(std::string name): Uniform(name) 
	{

	}

	void loadMatrix(GLuint programID, Matrix4 matrix) 
	{
		glUniformMatrix4fv(getLocation(programID), 1, GL_FALSE, matrix.ToArray());
	}
};

#endif