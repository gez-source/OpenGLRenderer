#ifndef JOINT_TRANSFORM_DATA_H
#define JOINT_TRANSFORM_DATA_H

#include <string>
#include "Matrix4.h"

/**
 * This contains the data for a transformation of one joint, at a certain time
 * in an animation. It has the name of the joint that it refers to, and the
 * local transform of the joint in the pose position.
 *
 * @author Karl
 *
 */
class JointTransformData
{
public:
	std::string jointNameId;
	Matrix4 jointLocalTransform;

	JointTransformData(std::string jointNameId, Matrix4 jointLocalTransform)
	{
		this->jointNameId = jointNameId;
		this->jointLocalTransform = jointLocalTransform;
	}
};


#endif