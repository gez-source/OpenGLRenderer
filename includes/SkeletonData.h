#ifndef SKELETON_DATA_H
#define SKELETON_DATA_H

#include "JointData.h"

class SkeletonData
{
public:

	int jointCount;
	JointData headJoint;

	SkeletonData() : jointCount(0)
	{

	}

	SkeletonData(int jointCount, JointData headJoint)
	{
		this->jointCount = jointCount;
		this->headJoint = headJoint;
	}

};


#endif