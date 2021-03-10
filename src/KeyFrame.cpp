#include "KeyFrame.h"

/**
 * @return The time in seconds of the keyframe in the animation.
 */
float KeyFrame::getTimeStamp()
{
	return timeStamp;
}

/**
 * @return The desired bone-space transforms of all the joints at this
 *         keyframe, of the animation, indexed by the name of the joint that
 *         they correspond to. This basically represents the "pose" at this
 *         keyframe.
 */
std::map<std::string, JointTransform*>* KeyFrame::getJointKeyFrames()
{
	return pose;
}