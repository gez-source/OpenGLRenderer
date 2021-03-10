#ifndef SKYBOX_H
#define SKYBOX_H

#include "Cubemap.h"
#include "Transform.h"
#include "Vao.h"
#include "ShaderProgram.h"
#include "UniformMatrix.h"
#include "UniformSampler.h"
#include "OpenGlUtils.h"

class Skybox 
{
private:
	Texture* cubemap;
	Vao* vao;
	ShaderProgram* shader;

	UniformMatrix* projectionMatrix = new UniformMatrix("projectionMatrix");
	UniformMatrix* viewMatrix = new UniformMatrix("viewMatrix");
	UniformSampler* cubeMap = new UniformSampler("cubemap");
public:
	Skybox(std::string texFileRightPosX, std::string texFileLeftNegX,
		std::string texFileTopPosY, std::string texFileBottomNegY,
		std::string texFileBackPosZ, std::string texFileFrontNegZ);

	~Skybox();

	void Render(Transform cameraTransform, Matrix4 projection);
};

#endif