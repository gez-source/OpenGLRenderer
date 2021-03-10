#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>
#include "KeyFrame.h"

/**
 *
 * * Represents an animation that can applied to an {@link AnimatedModel} . It
 * contains the length of the animation in seconds, and a list of
 * {@link KeyFrame}s.
 *
 * @author Karl
 *
 *
 */
class Animation 
{
private:
	float length; // in seconds
	std::vector<KeyFrame*>* keyFrames = new std::vector<KeyFrame*>();

public:
	/**
	 * @param lengthInSeconds
	 *            - the total length of the animation in seconds.
	 * @param frames
	 *            - all the keyframes for the animation, ordered by time of
	 *            appearance in the animation.
	 */
	Animation(float lengthInSeconds, std::vector<KeyFrame*>* frames)
	{
		this->keyFrames = frames;
		this->length = lengthInSeconds;
	}

	~Animation() 
	{
		for (KeyFrame* frame : *keyFrames)
		{
			delete frame;
		}
		delete keyFrames;
	}

	/**
	 * @return The length of the animation in seconds.
	 */
	float getLength();

	/**
	 * @return An array of the animation's keyframes. The array is ordered based
	 *         on the order of the keyframes in the animation (first keyframe of
	 *         the animation in array position 0).
	 */
	std::vector<KeyFrame*>* getKeyFrames();

};


#endif