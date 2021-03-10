#ifndef UNIFORM_SAMPLER_H
#define UNIFORM_SAMPLER_H

#include "Uniform.h"

class UniformSampler : public Uniform 
{
private:
	int currentValue;
	bool used = false;

public:
	UniformSampler(std::string name): Uniform(name), currentValue(-1)
	{
		
	}

	void loadTexUnit(GLuint programID, int texUnit)
	{
		if (!used || currentValue != texUnit) 
		{
			glUniform1i(getLocation(programID), texUnit);
			used = true;
			currentValue = texUnit;
		}
	}

};

#endif