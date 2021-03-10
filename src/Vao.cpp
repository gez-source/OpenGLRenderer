#include "Vao.h"

Vao::Vao()
{

}

Vao::Vao(GLuint id)
{
	this->id = id;
}

Vao::~Vao()
{
	glDeleteVertexArrays(1, &id);
	for (Vbo* vbo : dataVbos)
	{
		delete vbo;
	}
	delete indexVbo;
}

Vao* Vao::create()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	return new Vao(id);
}

int Vao::getIndexCount()
{
	return indexCount;
}

void Vao::createIndexBuffer(std::vector<int> indices)
{
	this->indexVbo = Vbo::create(GL_ELEMENT_ARRAY_BUFFER);
	indexVbo->bind();
	indexVbo->storeData(indices);
	this->indexCount = indices.size();
}

void Vao::createAttribute(int attribute, std::vector<Matrix4> data)
{
	const int ARRAY_SIZE = data.size() * sizeof(GLfloat) * 16;

	std::vector<float> matrices;
	
	for (int i = 0; i < data.size(); i++)
	{
		Matrix4 m = data[i];

		for (int j = 0; j < 16; j++)
		{
			matrices.push_back(m[j]);
		}
	}


	Vbo* dataVbo = Vbo::create(GL_ARRAY_BUFFER);
	dataVbo->bind();
	glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE, &matrices[0], GL_STATIC_DRAW);

	GLsizei vec4Size = sizeof(GLfloat) * 4;
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(attribute + 1);
	glVertexAttribPointer(attribute + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
	glEnableVertexAttribArray(attribute + 2);
	glVertexAttribPointer(attribute + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(attribute + 3);
	glVertexAttribPointer(attribute + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(attribute, 1);
	glVertexAttribDivisor(attribute + 1, 1);
	glVertexAttribDivisor(attribute + 2, 1);
	glVertexAttribDivisor(attribute + 3, 1);

	dataVbo->unbind();
	dataVbos.push_back(dataVbo);
}

void Vao::createAttribute(int attribute, std::vector<float> data, int attrSize)
{
	Vbo* dataVbo = Vbo::create(GL_ARRAY_BUFFER);
	dataVbo->bind();
	dataVbo->storeData(data);
	glVertexAttribPointer(attribute, attrSize, GL_FLOAT, GL_FALSE, attrSize * BYTES_PER_FLOAT, 0);
	//glVertexAttribPointer(attribute, attrSize, GL_FLOAT, GL_FALSE, 0, nullptr);
	dataVbo->unbind();
	dataVbos.push_back(dataVbo);
}

void Vao::createIntAttribute(int attribute, std::vector<int> data, int attrSize)
{
	Vbo* dataVbo = Vbo::create(GL_ARRAY_BUFFER);
	dataVbo->bind();
	dataVbo->storeData(data);
	glVertexAttribIPointer(attribute, attrSize, GL_INT, attrSize * BYTES_PER_INT, 0);
	//glVertexAttribIPointer(attribute, attrSize, GL_INT, 0, nullptr);
	dataVbo->unbind();
	dataVbos.push_back(dataVbo);
}

void Vao::bind()
{
	glBindVertexArray(id);
}

void Vao::unbind()
{
	glBindVertexArray(0);
}

void binder(GLuint start, GLuint end)
{
	glEnableVertexAttribArray(start);

	if (start < end)
	{
		binder(start + 1, end);
	}
}

void unbinder(GLuint start, GLuint end)
{
	glEnableVertexAttribArray(start);

	if (start < end)
	{
		binder(start + 1, end);
	}
}

void Vao::bind(GLuint startAttributeIndex, GLuint endAttributeIndex)
{
	bind();
	binder(startAttributeIndex, endAttributeIndex);
}

void Vao::unbind(GLuint startAttributeIndex, GLuint endAttributeIndex)
{
	unbind();
	unbinder(startAttributeIndex, endAttributeIndex);
}