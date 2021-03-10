#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <map>
#include <string>
#include "JointTransform.h"

/**
 *
 * Represents one keyframe of an animation. This contains the timestamp of the
 * keyframe, which is the time (in seconds) from the start of the animation when
 * this keyframe occurs.
 *
 * It also contains the desired bone-space transforms of all of the joints in
 * the animated entity at this keyframe in the animation (i.e. it contains all
 * the joint transforms for the "pose" at this time of the animation.). The
 * joint transforms are stored in a map, indexed by the name of the joint that
 * they should be applied to.
 *
 * @author Karl
 *
 */
class KeyFrame
{
private:
	float timeStamp;
	std::map<std::string, JointTransform*>* pose;

public:
	KeyFrame() : timeStamp(0.0f)
	{
		
	}

	~KeyFrame() 
	{
		delete pose;
	}

	/**
	 * @param timeStamp
	 *            - the time (in seconds) that this keyframe occurs during the
	 *            animation.
	 * @param jointKeyFrames
	 *            - the local-space transforms for all the joints at this
	 *            keyframe, indexed by the name of the joint that they should be
	 *            applied to.
	 */
	KeyFrame(float timeStamp, std::map<std::string, JointTransform*>* jointKeyFrames)
	{
		this->timeStamp = timeStamp;
		this->pose = jointKeyFrames;
	}

	/**
	 * @return The time in seconds of the keyframe in the animation.
	 */
	float getTimeStamp();

	/**
	 * @return The desired bone-space transforms of all the joints at this
	 *         keyframe, of the animation, indexed by the name of the joint that
	 *         they correspond to. This basically represents the "pose" at this
	 *         keyframe.
	 */
	std::map<std::string, JointTransform*>* getJointKeyFrames();

};


#endif