#ifndef XML_SKIN_LOADER_H
#define XML_SKIN_LOADER_H

#include <string>
#include <vector>
#include "SkinningData.h"
#include "StringHelpers.h"
#include "pugixml.hpp"
#include "XMLHelpers.h"

class SkinLoader 
{
private:
	pugi::xpath_node skinningData;
	pugi::xpath_node visualScene;
	int maxWeights;

public:
	SkinLoader(pugi::xpath_node libraryControllers, pugi::xpath_node libraryVisualScenes, int maxWeights);
	SkinningData extractSkinData();

private:

	void loadJointsList(std::vector<std::string>& jointIDList, std::vector<std::string>& jointSIDList);
	float* loadWeights(int& count);
	int* getEffectiveJointsCounts(int& count, pugi::xpath_node weightsDataNode);
	std::vector<VertexSkinData> getSkinData(pugi::xpath_node weightsDataNode, int* counts, int countJoints, float* weights, int countWeights);
	std::vector<Matrix4> loadBindPoses();
};

#endif