#include "Joint.h"

void Joint::addChild(Joint* child)
{
	if (child != nullptr)
	{
		child->parent = this;
		this->children.push_back(child);
	}
}

Matrix4 Joint::getLocalBindTransform()
{
	return localBindTransform;
}

Matrix4 Joint::getAnimatedTransform()
{
	return animatedTransform;
}

void Joint::setAnimationTransform(Matrix4 animationTransform)
{
	this->animatedTransform = animationTransform;
}

Matrix4 Joint::getInverseBindTransform()
{
	return inverseBindTransformCalc;
	//return inverseBindTransform;
}

void Joint::calcInverseBindTransform(Matrix4 parentBindTransform)
{
	Matrix4 bindTransform = parentBindTransform * localBindTransform;

	inverseBindTransformCalc = bindTransform.Inverse();

	for (Joint* child : children)
	{
		child->calcInverseBindTransform(bindTransform);
	}
}