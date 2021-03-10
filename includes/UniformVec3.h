#ifndef UNIFORM_VEC3_H
#define UNIFORM_VEC3_H

#include "Uniform.h"
#include "Vector3.h"

class UniformVec3 : public Uniform 
{
private:
	float x;
	float y;
	float z;
	bool used = false;

public:
	UniformVec3(std::string name) : Uniform(name), x(0.0f), y(0.0f), z(0.0f)
	{
		
	}

	void loadVec3(GLuint programID, Vector3 vector)
	{
		loadVec3(programID, vector.x, vector.y, vector.z);
	}

	void loadVec3(GLuint programID, float x, float y, float z)
	{
		if (!used || this->x != x || this->y != y || this->z != z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			used = true;
			glUniform3f(getLocation(programID), x, y, z);
		}
	}

};

#endif