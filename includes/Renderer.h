// Gerallt 2017.

#ifndef RENDERER_H
#define RENDERER_H

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
#include "Character.h"
#include "AnimatedModel.h"
#include "AnimatedModelRenderer.h"
#include "Skybox.h"
#include "Terrain.h"
#include "FileWatcher.h"
#include "MessageQueue.h"
#include "OctreeRenderer.h"

class Renderer
{
private:
	int fov = 90;
	GLuint programScreenQuad = 0;
	GLuint programScreenQuadAO = 0;
	GLuint programCursor = 0;
	GLuint vaoScreenQuad = 0, cursorVAO = 0;
	Vao* vaoCursor;
	unsigned int hdrFBO;
	unsigned int rboDepth;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	ShaderProgram* shaderBlur;
	ShaderProgram* shaderBloomFinal;
	bool bloom = true;
	float exposure = 1.0f;

	int screenQuadVertexCount = 0;
	bool noErrors = true;
	Texture* textureCursor;
	Character* floor;
	Character* character00;
	Character* character01;
	Character* character02;
	AnimatedModel* animModel;
	Texture* animDiffuseTexture;
	Texture* animNormalTexture;
	Texture* animSpecularTexture;
	Texture* lutTexture00;
	Texture* lutTexture01;
	Texture* lutTexture02;
	Texture* lutTexture03;
	Texture* lutTexture04;
	Animation* selectedAnimation;
	Animator* animator;
	AnimatedModelRenderer* animRenderer;
	Skybox* skybox;
	Terrain* terrain;
	OctreeRenderer* octreeRenderer;
	FileWatcher* fw;
	MessageQueue* messageQueue;
	GLuint noiseTexture;
	unsigned char* noisePixelData;
	float freq = SAMPLE_FREQ;
	float amp = SAMPLE_AMP;
	float pers = 0.1f;
	int octaves = 10;
	float scaleX = 0.1f;
	float scaleY = 0.1f;
	float scaleZ = 0.1f;
public:
	sf::RenderWindow* window;
	bool hasFocus;
	Matrix4 projectionMatrix;
	Matrix4 modelview;
	Matrix4 lightModelView;
	Vector3 lightDir = Vector3(0, 0, 0);

	Renderer();
	~Renderer();

	void Initilise();
	bool InitGL();
	void OnResize(int width, int height);
	void LoadModels();
	void LoadShaders();
	void renderQuad();
	void Render(Transform cameraTransform);
	void InitFBOs();
	void InitScene();
	void InitScreenQuad();
	void initQuad();
	void ShowShadowMap(Transform cameraTransform);
	void ShowAOMap();
	void InitCursor();
	void RenderCursor(Transform cameraTransform);
	void InitNoiseMap();
	void ShowNoiseMap();
};

#endif
