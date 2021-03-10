#ifndef UNIFORM_VEC3_ARR_H
#define UNIFORM_VEC3_ARR_H

#include <vector>
#include "Uniform.h"
#include "Vector3.h"
#include "UniformMatrix.h"

class UniformVec3Array : public Uniform
{
public:
	UniformVec3Array(std::string name, int size) : Uniform(name)
	{

	}

	void storeUniformLocation(GLuint programID) override
	{
		Uniform::storeUniformLocation(programID);
	}

	void loadVectorArray(GLuint programID, std::vector<Vector3> vectors)
	{
		int VECTOR_SIZE = 3;
		int i;
		int j;
		int k;
		int bufferSize = VECTOR_SIZE * vectors.size();
		float* floatBuffer = new float[bufferSize];
		Vector3 v;
		float* vector;

		for (i = 0; i < vectors.size(); i++)
		{
			j = i * VECTOR_SIZE;
			v = vectors[i];
			vector = v.ToArray();

			for (k = 0; k < VECTOR_SIZE; k++)
			{
				floatBuffer[j + k] = vector[k];
			}
		}
		
		glUniform3fv(getLocation(programID), vectors.size(), floatBuffer);
	}

};


#endif