#ifndef UNIFORM_MAT4_ARR_H
#define UNIFORM_MAT4_ARR_H

#include "Uniform.h"
#include "Matrix4.h"
#include "UniformMatrix.h"

class UniformMat4Array : public Uniform
{
public:
	UniformMat4Array(std::string name, int size) : Uniform(name)
	{

	}

	void storeUniformLocation(GLuint programID) override
	{
		Uniform::storeUniformLocation(programID);
	}

	void loadMatrixArray(GLuint programID, std::vector<Matrix4> matrices)
	{
		loadMatrix(programID, matrices);
	}

	void loadMatrix(GLuint programID, std::vector<Matrix4> matrixArray)
	{
		int MATRIX_SIZE = 4 * 4;
		int i;
		int j;
		int k;
		int bufferSize = MATRIX_SIZE * matrixArray.size();
		float* floatBuffer = new float[bufferSize];
		Matrix4 m;
		float* matrix;

		for (i = 0; i < matrixArray.size(); i++)
		{
			j = i * MATRIX_SIZE;
			m = matrixArray[i];
			matrix = m.ToArray();

			for (k = 0; k < MATRIX_SIZE; k++)
			{
				floatBuffer[j + k] = matrix[k];
			}
		}

		glUniformMatrix4fv(getLocation(programID), matrixArray.size(), GL_FALSE, floatBuffer);
	}

};


#endif