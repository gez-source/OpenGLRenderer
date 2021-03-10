#include "KeyFrameData.h"

void KeyFrameData::addJointTransform(JointTransformData* transform)
{
	jointTransforms->push_back(transform);
}