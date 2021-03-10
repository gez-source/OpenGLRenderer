#ifndef VAO_H
#define VAO_H

#include <vector>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include "Vbo.h"
#include "Matrix4.h"

class Vao
{
private:
	int BYTES_PER_FLOAT = sizeof(float);
	int BYTES_PER_INT = sizeof(int);
	std::vector<Vbo*> dataVbos = std::vector<Vbo*>();
	Vbo* indexVbo;
	int indexCount;

public:
	GLuint id;

	Vao();
	Vao(GLuint id);
	~Vao();

	static Vao* create();

	int getIndexCount();
	void createIndexBuffer(std::vector<int> indices);
	void createAttribute(int attribute, std::vector<Matrix4> data);
	void createAttribute(int attribute, std::vector<float> data, int attrSize);
	void createIntAttribute(int attribute, std::vector<int> data, int attrSize);
	void bind();
	void unbind();
	void bind(GLuint startAttributeIndex, GLuint endAttributeIndex);
	void unbind(GLuint startAttributeIndex, GLuint endAttributeIndex);

};

#endif