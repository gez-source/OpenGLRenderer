#include "Vbo.h"

Vbo::Vbo() : vboId(0), type(-1)
{

}

Vbo::Vbo(int vboId, int type)
{
	this->vboId = vboId;
	this->type = type;
}

Vbo::~Vbo()
{
	glDeleteBuffers(1, &vboId);
}

Vbo* Vbo::create(int type)
{
	GLuint id;
	glGenBuffers(1, &id);
	return new Vbo(id, type);
}

void Vbo::bind()
{
	glBindBuffer(type, vboId);
}

void Vbo::unbind()
{
	glBindBuffer(type, 0);
}

void Vbo::storeData(std::vector<int> data)
{
	if (data.size() == 0) return;
	glBufferData(type, sizeof(int) * data.size(), &data[0], GL_STATIC_DRAW);
}

void Vbo::storeData(std::vector<float> data)
{
	if (data.size() == 0) return;
	glBufferData(type, sizeof(GLfloat) * data.size(), &data[0], GL_STATIC_DRAW);
}