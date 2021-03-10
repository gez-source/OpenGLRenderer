#ifndef SKINNING_DATA_H
#define SKINNING_DATA_H

#include <vector>
#include <string>
#include "VertexSkinData.h"
#include "Matrix4.h"

class SkinningData 
{
public:
	std::vector<std::string> jointOrder;
	std::vector<std::string> jointSIDList;
	std::vector<VertexSkinData> verticesSkinData;
	std::vector<Matrix4> bindPoses;

	SkinningData() 
	{

	}

	SkinningData(std::vector<std::string> jointOrder, std::vector<std::string> jointSIDList, std::vector<VertexSkinData> verticesSkinData, std::vector<Matrix4> bindPoses)
	{
		this->jointOrder = jointOrder;
		this->jointSIDList = jointSIDList;
		this->verticesSkinData = verticesSkinData;
		this->bindPoses = bindPoses;
	}
};

#endif