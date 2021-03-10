#include "AnimatedModelRenderer.h"

AnimatedModelRenderer::AnimatedModelRenderer()
{
	this->shader = new AnimatedModelShader();
}

AnimatedModelRenderer::~AnimatedModelRenderer()
{
	delete shader;
}

void AnimatedModelRenderer::render(AnimatedModel* entity, Transform camera, Matrix4 projection, Matrix4 modelview, Vector3 lightDir)
{
	if (entity->getRootJoint() == nullptr)
	{
		shader->start();
	}
	else 
	{
		shader->startEnableAnimations();
	}

	prepare(camera, projection, modelview, lightDir);
	entity->getModel()->bind(0, 6);
	shader->jointTransforms->loadMatrixArray(shader->programID, entity->getJointTransforms());
	glDrawElements(GL_TRIANGLES, entity->getModel()->getIndexCount(), GL_UNSIGNED_INT, nullptr);
	entity->getModel()->unbind(0, 6);
	finish();
}

void AnimatedModelRenderer::prepare(Transform camera, Matrix4 projection, Matrix4 modelview, Vector3 lightDir)
{
	shader->projectionMatrix->loadMatrix(shader->programID, projection);
	shader->viewMatrix->loadMatrix(shader->programID, camera.localMatrix);
	shader->modelviewMatrix->loadMatrix(shader->programID, modelview);
	shader->lightDirection->loadVec3(shader->programID, lightDir);
	shader->eye->loadVec3(shader->programID, camera.localMatrix.GetTranslation());
	OpenGlUtils::Antialias(true);
	OpenGlUtils::DisableBlending();
	OpenGlUtils::EnableDepthTesting(true);
	//OpenGlUtils::GoWireframe(true);
}

void AnimatedModelRenderer::finish()
{
	shader->stop();
}