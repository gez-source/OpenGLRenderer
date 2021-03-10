#ifndef UNIFORM_INT_H
#define UNIFORM_INT_H

#include "Uniform.h"

class UniformInt : public Uniform
{
private:
	int value;
	bool used = false;

public:
	UniformInt(std::string name) : Uniform(name), value(0)
	{
		
	}

	void loadInt(GLuint programID, int value)
	{
		if (!used || this->value != value)
		{
			this->value = value;
			used = true;
			glUniform1i(getLocation(programID), value);
		}
	}
};

#endif