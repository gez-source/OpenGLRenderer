#ifndef XML_SKELETON_LOADER_H
#define XML_SKELETON_LOADER_H

#include <string>
#include <vector>
#include "pugixml.hpp"
#include "Matrix4.h"
#include "SkeletonData.h"
#include "XMLHelpers.h"
#include "StringHelpers.h"

class SkeletonLoader 
{
private:
	pugi::xpath_node armatureData;
	std::vector<std::string> boneOrder;
	std::vector<Matrix4> bindPoses;
	int jointCount = 0;

public:
	SkeletonLoader(pugi::xpath_node jointHierarchy, std::vector<std::string> boneOrder, std::vector<Matrix4> bindPoses);
	SkeletonData extractBoneData(bool& loaded);

private:
	JointData loadJointData(pugi::xpath_node jointNode, bool isRoot);
	JointData extractMainJointData(pugi::xpath_node jointNode, bool isRoot);
};

#endif