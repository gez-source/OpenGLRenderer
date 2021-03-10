#include "VertexSkinData.h"

void VertexSkinData::addJointEffect(int jointId, float weight)
{
	for (int i = 0; i < weights.size(); i++)
	{
		if (weight > weights.at(i))
		{
			jointIds.at(i) = jointId;
			weights.at(i) = weight;

			//std::vector<int>::iterator itJoint = jointIds.begin() + i;
			//std::vector<float>::iterator itWeight = weights.begin() + i;
			//jointIds.insert(itJoint, jointId);
			//weights.insert(itWeight, weight);
			return;
		}
	}
	jointIds.push_back(jointId);
	weights.push_back(weight);
}

void VertexSkinData::limitJointNumber(int max)
{
	if (jointIds.size() > max)
	{
		std::vector<float> topWeights = std::vector<float>(max);
		float total = saveTopWeights(topWeights);
		refillWeightList(topWeights, total);
		removeExcessJointIds(max);
	}
	else if (jointIds.size() < max)
	{
		fillEmptyWeights(max);
	}
}

void VertexSkinData::fillEmptyWeights(int max)
{
	while (jointIds.size() < max)
	{
		jointIds.push_back(0);
		weights.push_back(0.0f);
	}
}

float VertexSkinData::saveTopWeights(std::vector<float>& topWeightsArray)
{
	float total = 0;
	for (int i = 0; i < topWeightsArray.size(); i++)
	{
		topWeightsArray[i] = weights.at(i);
		total += topWeightsArray[i];
	}
	return total;
}

void VertexSkinData::refillWeightList(std::vector<float>& topWeights, float total)
{
	weights.clear();
	for (int i = 0; i < topWeights.size(); i++)
	{
		weights.push_back(std::min(topWeights[i] / total, 1.0f));
	}
}

void VertexSkinData::removeExcessJointIds(int max)
{
	while (jointIds.size() > max)
	{
		//jointIds.erase(jointIds.begin() + jointIds.size() - 1);
		jointIds.erase(jointIds.end() - 1);
	}
}