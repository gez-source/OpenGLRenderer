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
#include "FileWatcher.h"
#include "MessageQueue.h"
#include "VirtualWorld.h"

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
	VirtualWorld* virtualWorld;

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
	Vector3 worldPosition = Vector3::Zero;

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
