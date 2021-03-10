#ifndef ANIMATED_MODEL_DATA_H
#define ANIMATED_MODEL_DATA_H

#include "SkeletonData.h"
#include "MeshData.h"
#include "AnimationData.h"

/**
 * Contains the extracted data for an animated model, which includes the mesh data, and skeleton (joints heirarchy) data.
 * @author Karl
 *
 */
class AnimatedModelData 
{
private:
	SkeletonData joints;
	MeshData* mesh;
	AnimationData* animData;
public:

	AnimatedModelData() : animData(nullptr), mesh(nullptr)
	{

	}

	AnimatedModelData(MeshData* mesh, SkeletonData joints, AnimationData* animData)
	{
		this->joints = joints;
		this->mesh = mesh;
		this->animData = animData;
	}

	~AnimatedModelData()
	{
		delete mesh;
	}

	SkeletonData getJointsData();
	MeshData* getMeshData();
	AnimationData* getAnimationData();
};

#endif