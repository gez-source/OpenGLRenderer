#include "Animation.h"

float Animation::getLength()
{
	return length;
}

std::vector<KeyFrame*>* Animation::getKeyFrames()
{
	return keyFrames;
}