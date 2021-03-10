#ifndef UNIFORM_H
#define UNIFORM_H

#include <string>
#include <iostream>
#include <GL/glew.h>

class Uniform 
{
private:
	int NOT_FOUND = -1;

	std::string name;
	int location;
	bool invalid = false;

public:
	Uniform() : location(NOT_FOUND)
	{
		
	}

	Uniform(std::string name) : location(-1)
	{
		this->name = name;
	}

	virtual void storeUniformLocation(GLuint programID)
	{
		location = glGetUniformLocation(programID, name.c_str());
		if (!invalid && location == NOT_FOUND)
		{
			std::cout << "No uniform variable called " << name << " found!" << std::endl;
			invalid = true;
		}
	}

	GLuint getLocation(GLuint programID)
	{
		location = glGetUniformLocation(programID, name.c_str());
		return location;
	}

};

#endif