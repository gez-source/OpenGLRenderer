#include "AnimatedModel.h"
#include <stack>

AnimatedModel::AnimatedModel(Vao* model, Joint* rootJoint, int jointCount, AnimatedModelData* entityData, Animation* animation)
{
	this->model = model;
	this->rootJoint = rootJoint;
	this->jointCount = jointCount;
	this->entityData = entityData;
	this->texture = nullptr;
	this->animation = animation;

	// Compute inverse bind poses (do this if they are not already pre-loaded)
	if(rootJoint != nullptr)
	{
		rootJoint->calcInverseBindTransform(Matrix4::GetIdentity());
	}
}

AnimatedModel::~AnimatedModel()
{
	delete model;

	if (this->animation != nullptr)
	{
		delete animation;
	}

	delete this->entityData;
	delete this->rootJoint;
}

Vao* AnimatedModel::getModel()
{
	return model;
}

Joint* AnimatedModel::getRootJoint()
{
	return rootJoint;
}

Animation* AnimatedModel::getAnimation()
{
	return animation;
}

std::vector<Matrix4> AnimatedModel::getJointTransforms()
{
	std::stack<Joint*> stack;
	std::vector<Matrix4> jointMatrices = std::vector<Matrix4>(jointCount);

	if (jointCount > 0)
	{
		stack.push(rootJoint);

		while (!stack.empty())
		{
			Joint* jointNode = stack.top();
			stack.pop();

			jointMatrices.at(jointNode->index) = jointNode->getAnimatedTransform();

			for (Joint* childJoint : jointNode->children)
			{
				stack.push(childJoint);
			}
		}
	}
	return jointMatrices;
}