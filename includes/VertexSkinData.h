#ifndef VERTEX_SKIN_DATA_H
#define VERTEX_SKIN_DATA_H

#include <vector>
#include <algorithm>

class VertexSkinData 
{
public:
	std::vector<int> jointIds = std::vector<int>();
	std::vector<float> weights = std::vector<float>();

	void addJointEffect(int jointId, float weight);
	void limitJointNumber(int max);

private:
	void fillEmptyWeights(int max);
	float saveTopWeights(std::vector<float>& topWeightsArray);
	void refillWeightList(std::vector<float>& topWeights, float total);
	void removeExcessJointIds(int max);

};

#endif