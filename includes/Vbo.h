#ifndef VBO_H
#define VBO_H

#include <vector>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

class Vbo 
{
private:
	GLuint vboId;
	int type;

public:
	Vbo();
	Vbo(int vboId, int type);
	~Vbo();
	static Vbo* create(int type);

	void bind();
	void unbind();
	void storeData(std::vector<int> data);
	void storeData(std::vector<float> data);
};

#endif