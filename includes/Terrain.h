#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "Vao.h"
#include "Cubemap.h"
#include "ShaderProgram.h"
#include "UniformMatrix.h"
#include "UniformSampler.h"
#include "UniformMat4Array.h"
#include "UniformVec3Array.h"
#include "NoiseHelpers.h"
#include "UniformVec3.h"
#include "Transform.h"
#include "OpenGlUtils.h"
#include "Light.h"

class Terrain 
{
private:
	int MAX_CHUNKS = 80000;
	int MAX_LIGHTS = 32;

	float t = 0;

	Vao* vao;
	Texture* cubemapDiffuse;
	Texture* cubemapSpecular;
public:
	Texture* lutTexture00;
	Texture* lutTexture01;
	Texture* lutTexture02;
	Texture* lutTexture03;
	Texture* lutTexture04;

private:
	ShaderProgram* shader;

	UniformMatrix* projectionMatrix = new UniformMatrix("projectionMatrix");
	UniformMatrix* viewMatrix = new UniformMatrix("viewMatrix");
	UniformMatrix* modelMatrix = new UniformMatrix("modelMatrix");
	UniformSampler* cubeMap = new UniformSampler("cubemap");
	UniformVec3* lightDirection = new UniformVec3("lightDirection");

	Matrix4 model;
	std::vector<Matrix4> transforms;
	std::vector<Light> lights;

public:

	Terrain();

	~Terrain();

	void Initilise();
	void InitiliseLights();

	void Render(Transform cameraTransform, Matrix4 projection, Vector3 lightDir);
	void renderCube(Transform cameraTransform, Matrix4 projection, Matrix4 model);
	void renderCubes_slow(Transform cameraTransform, Matrix4 projection, Matrix4 model);
	void renderCubes_instanced(Transform cameraTransform, Matrix4 projection, Matrix4 model, Vector3 lightDir);
};

#endif