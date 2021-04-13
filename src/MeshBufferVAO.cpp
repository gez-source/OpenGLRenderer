#include "MeshBufferVAO.h"

MeshBufferVAO::MeshBufferVAO()
{
	
}

MeshBufferVAO::~MeshBufferVAO()
{
	if (model != nullptr)
	{
		delete model;
	}
	if (Bounds != nullptr)
	{
		delete Bounds;
	}

	//TODO: free VAO and mesh 
}


void MeshBufferVAO::LoadModelBuffers(Model* model,
	std::vector<GLfloat>& verticies,
	std::vector<GLfloat>& texCoords,
	std::vector<GLfloat>& normals,
	std::vector<GLfloat>& tangents,
	std::vector<GLfloat>& bitangents,
	int& verticiesCount)
{
	int i;
	int j;
	Vector3 tangent;
	Vector3 bitangent;

	Bounds = new AABB();

	for (i = 0; i < model->nfaces(); i++)
	{
		// Vertex Coordinate
		Vector3 v0 = model->vert(i, 0);
		Vector3 v1 = model->vert(i, 1);
		Vector3 v2 = model->vert(i, 2);

		Bounds->Add(v0);
		Bounds->Add(v1);
		Bounds->Add(v2);

		verticies.push_back(v0.x);
		verticies.push_back(v0.y);
		verticies.push_back(v0.z);

		verticies.push_back(v1.x);
		verticies.push_back(v1.y);
		verticies.push_back(v1.z);

		verticies.push_back(v2.x);
		verticies.push_back(v2.y);
		verticies.push_back(v2.z);

		// Texture Coordinates
		Vector2 t0 = model->uv(i, 0);
		Vector2 t1 = model->uv(i, 1);
		Vector2 t2 = model->uv(i, 2);

		texCoords.push_back(t0.x);
		texCoords.push_back(t0.y);

		texCoords.push_back(t1.x);
		texCoords.push_back(t1.y);

		texCoords.push_back(t2.x);
		texCoords.push_back(t2.y);

		// Normal Coordinates
		Vector3 n0 = model->normal(i, 0);
		Vector3 n1 = model->normal(i, 1);
		Vector3 n2 = model->normal(i, 2);

		// Tangents and Bitangents
		TriangleTangentSpace(v0, v1, v2, t0, t1, t2, tangent, bitangent);

		//Vector3 normal;
		//ComputeTangentBasis(v0, v1, v2, t0, t1, t2, normal, tangent, bitangent);
		//n0 = normal;
		//n1 = normal;
		//n2 = normal;

		normals.push_back(n0.x);
		normals.push_back(n0.y);
		normals.push_back(n0.z);

		normals.push_back(n1.x);
		normals.push_back(n1.y);
		normals.push_back(n1.z);

		normals.push_back(n2.x);
		normals.push_back(n2.y);
		normals.push_back(n2.z);

		for (j = 0; j <= 2; j++)
		{
			// Add the same data for each vertex 

			tangents.push_back(tangent.x);
			tangents.push_back(tangent.y);
			tangents.push_back(tangent.z);

			bitangents.push_back(bitangent.x);
			bitangents.push_back(bitangent.y);
			bitangents.push_back(bitangent.z);
		}
	}

	verticiesCount += model->nfaces() * 3;

	Bounds->Center = Bounds->GetCenter();
	Bounds->HalfSize = Bounds->GetHalfSize();

}

void MeshBufferVAO::LoadModel(std::string fileNameModel)
{
	std::vector<GLfloat> verticies;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> tangents;
	std::vector<GLfloat> bitangents;

	model = new Model(fileNameModel);

	LoadModelBuffers(model, verticies, texCoords, normals, tangents, bitangents, verticiesCount);

	GLuint points_vbo;
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * verticies.size(), &verticies[0], GL_STATIC_DRAW);

	GLuint texcoords_vbo;
	glGenBuffers(1, &texcoords_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

	GLuint normals_vbo;
	glGenBuffers(1, &normals_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

	GLuint tangents_vbo;
	glGenBuffers(1, &tangents_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tangents.size(), &tangents[0], GL_STATIC_DRAW);

	GLuint bitangents_vbo;
	glGenBuffers(1, &bitangents_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, bitangents_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * bitangents.size(), &bitangents[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, bitangents_vbo);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	//glEnableVertexAttribArray(3);
	//glEnableVertexAttribArray(4);
}

void MeshBufferVAO::LoadTextures()
{
	if (model->diffusemap_->textureData->getBuffer() != nullptr)
	{
		model->diffusemap_->bindToUnit(0);
		texture = model->diffusemap_->textureId;
	}

	if (model->normalmap_->textureData->getBuffer() != nullptr)
	{
		model->normalmap_->bindToUnit(1);
		textureNormalMap = model->normalmap_->textureId;
	}

	if (model->specularmap_->textureData->getBuffer() != nullptr)
	{
		model->specularmap_->bindToUnit(1);
		textureSpecularMap = model->specularmap_->textureId;
	}

	if (model->glowmap_->textureData->getBuffer() != nullptr)
	{
		model->glowmap_->bindToUnit(1);
		textureGlowMap = model->glowmap_->textureId;
	}
}

void MeshBufferVAO::BindTextures()
{
	model->diffusemap_->bindToUnit(0);
	model->normalmap_->bindToUnit(1);
	model->specularmap_->bindToUnit(2);
	model->glowmap_->bindToUnit(3);
}

void MeshBufferVAO::Render()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glDrawArrays(GL_TRIANGLES, 0, verticiesCount);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glBindVertexArray(0);
}

bool MeshBufferVAO::Intersects(AABB* boundingBox)
{
	return this->Bounds->Intersects(boundingBox);
}