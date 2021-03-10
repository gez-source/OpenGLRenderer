#ifndef KEYFRAME_DATA_H
#define KEYFRAME_DATA_H

#include <vector>
#include "JointTransformData.h"

class KeyFrameData
{
public:
	float time;
	std::vector<JointTransformData*>* jointTransforms = new std::vector<JointTransformData*>();

	KeyFrameData() : time(0.0f)
	{
		
	}

	KeyFrameData(float time)
	{
		this->time = time;
	}

	~KeyFrameData() 
	{
		for (JointTransformData* jointTransformData : *jointTransforms)
		{
			delete jointTransformData;
		}
		delete jointTransforms;
	}

	void addJointTransform(JointTransformData* transform);

};


#endif