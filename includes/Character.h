#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "IOHelpers.h"
#include "model.h"
#include "Matrix4.h"
#include "Transform.h"
#include "MathHelpers.h"
#include "ShaderHelpers.h"
#include "TGADecoder.h"
#include "MeshBufferVAO.h"

class Character
{
private:
	ShaderProgram* program = nullptr;
	ShaderProgram* programShadowmap = nullptr;
	GLuint FramebufferName = 0;

public:
	MeshBufferVAO* meshBuffer = nullptr;
	GLuint depthTexture = 0;
	GLuint aoTexture = 0;
	Matrix4 depthMVP;

	Texture* lutTexture00;
	Texture* lutTexture01;
	Texture* lutTexture02;
	Texture* lutTexture03;
	Texture* lutTexture04;
public:
	Character() : meshBuffer(nullptr)
	{
		this->meshBuffer = new MeshBufferVAO();
		this->Initilise();
	}
	~Character()
	{

	}

	void LoadModel(std::string fileNameModel);
	void LoadShaders();
	void LoadTextures();

	void Initilise();
	void Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView);
	void RenderModel(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, bool outlinePass);

	void InitShadowMap();
	void RenderShadowMap(sf::RenderWindow* window, Transform cameraTransform, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView);

	void InitAOMap();
	void RenderAOMap(sf::RenderWindow* window, Transform cameraTransform, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView);
};

#endif