#ifndef MESH_BUFFER_VAO_H
#define MESH_BUFFER_VAO_H

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "model.h"
#include "MathHelpers.h"
#include "AABB.h"

class MeshBufferVAO
{
private:
	Model* model = nullptr;
	bool quads = false;
	int verticiesCount = 0;

	GLuint vao = 0;
	GLuint points_vbo;
	GLuint texcoords_vbo;
	GLuint normals_vbo;
	GLuint tangents_vbo;
	GLuint bitangents_vbo;

	GLuint texture = 0;
	GLuint textureNormalMap = 0;
	GLuint textureSpecularMap = 0;
	GLuint textureGlowMap = 0;
	
public:
	
	AABB* Bounds = nullptr;

	MeshBufferVAO();
	~MeshBufferVAO();

	void LoadModelBuffers(Model* model, std::vector<GLfloat>& verticies, std::vector<GLfloat>& texCoords,
		std::vector<GLfloat>& normals, std::vector<GLfloat>& tangents, std::vector<GLfloat>& bitangents, int& verticiesCount);

	void LoadModel(std::string fileNameModel);

	void LoadTextures();
	void BindTextures();
	void Render();

	/// <summary>
	/// Test if the this MeshBufferVAO intersects the specified bounding box.
	/// </summary>
	bool Intersects(AABB* boundingBox);
};

#endif