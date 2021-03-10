#ifndef ANIMATION_DATA_H
#define ANIMATION_DATA_H

#include <vector>
#include "KeyFrameData.h"

/**
 * Contains the extracted data for an animation, which includes the length of
 * the entire animation and the data for all the keyframes of the animation.
 *
 * @author Karl
 *
 */
class AnimationData
{
public:
	float lengthSeconds;
	std::vector<KeyFrameData*>* keyFrames = new std::vector<KeyFrameData*>();

	AnimationData() : lengthSeconds(0)
	{

	}

	~AnimationData() 
	{
		for (KeyFrameData* keyFrameData : *keyFrames)
		{
			delete keyFrameData;
		}
		delete keyFrames;
	}

	AnimationData(float lengthSeconds, std::vector<KeyFrameData*>* keyFrames)
	{
		this->lengthSeconds = lengthSeconds;
		this->keyFrames = keyFrames;
	}

};


#endif