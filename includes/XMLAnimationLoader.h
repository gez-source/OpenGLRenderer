#ifndef XML_ANIMATION_LOADER_H
#define XML_ANIMATION_LOADER_H

#include <string>
#include <vector>
#include "Matrix4.h"
#include "pugixml.hpp"
#include "AnimationData.h"
#include "StringHelpers.h"
#include "XMLHelpers.h"

class AnimationLoader 
{
private:
	pugi::xpath_node animationData;
	pugi::xpath_node jointHierarchy;

public:
	AnimationLoader(pugi::xpath_node animationData, pugi::xpath_node jointHierarchy);
	AnimationData* extractAnimation();

private:
	std::vector<float> getKeyTimes();
	std::vector<KeyFrameData*>* initKeyFrames(std::vector<float> times);
	void loadJointTransforms(std::vector<KeyFrameData*>* frames, pugi::xpath_node jointData, std::string rootNodeId);
	std::string getDataId(pugi::xpath_node jointData);
	std::string getJointName(pugi::xpath_node jointData);
	void processTransforms(std::string jointName, std::string rawData, std::vector<KeyFrameData*>* keyFrames, bool root);
	std::string findRootJointName();
	void checkNodeHierarchy();
};


#endif