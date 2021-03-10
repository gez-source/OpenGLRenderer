#include "AnimatedModelData.h"

SkeletonData AnimatedModelData::getJointsData()
{
	return joints;
}

MeshData* AnimatedModelData::getMeshData()
{
	return mesh;
}

AnimationData* AnimatedModelData::getAnimationData()
{
	return animData;
}