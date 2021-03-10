#include "XMLSkinLoader.h"
#include "AnimatedModelLoader.h"

SkinLoader::SkinLoader(pugi::xpath_node libraryControllers, pugi::xpath_node libraryVisualScenes, int maxWeights)
{
	this->skinningData = libraryControllers.node().select_node("//controller//skin");
	this->maxWeights = maxWeights;

	this->visualScene = libraryVisualScenes.node().select_node("//visual_scene");
}

SkinningData SkinLoader::extractSkinData()
{
	std::vector<std::string> jointIDList;
	std::vector<std::string> jointSIDList;
	loadJointsList(jointIDList, jointSIDList);

	int weightsCount;
	float* weights = loadWeights(weightsCount);

	pugi::xpath_node weightsDataNode = skinningData.node().child("vertex_weights");

	int jointsCount;
	int* effectorJointCounts = getEffectiveJointsCounts(jointsCount, weightsDataNode);

	std::vector<VertexSkinData> vertexWeights = getSkinData(weightsDataNode, effectorJointCounts, jointsCount, weights, weightsCount);

	std::vector<Matrix4> bindPoses = loadBindPoses();

	return SkinningData(jointIDList, jointSIDList, vertexWeights, bindPoses);
}

void SkinLoader::loadJointsList(std::vector<std::string>& jointIDList, std::vector<std::string>& jointSIDList)
{
	pugi::xpath_node inputNode = skinningData.node().child("vertex_weights");
	std::string jointDataId = std::string(GetChildWithAttribute(inputNode, "input", "semantic", "JOINT").node().attribute("source").value()).substr(1);
	pugi::xpath_node jointsNode = GetChildWithAttribute(skinningData, "source", "id", jointDataId).node().child("Name_array");

	std::vector<std::string> names;
	names = StringSplit(std::string(jointsNode.node().text().as_string()), " ");
	
	auto nodes = visualScene.node().select_nodes("//node");

	//for (pugi::xpath_node node : nodes)
	//{
	//	std::string node_id = std::string(node.node().attribute("id").value());
	//	std::string node_name = std::string(node.node().attribute("name").value());
	//	std::string node_sid = std::string(node.node().attribute("sid").value());

	//	//jointIDList.push_back(node_id);
	//	//jointSIDList.push_back(node_sid);
	//}

	for (std::string name : names)
	{
		// Lookup to find matching id, since blender changes to using the id.
		
		for (pugi::xpath_node node : nodes)
		{
			std::string node_id = std::string(node.node().attribute("id").value());
			std::string node_name = std::string(node.node().attribute("name").value());
			std::string node_sid = std::string(node.node().attribute("sid").value());

			if (name == node_sid)
			{
				jointIDList.push_back(node_id);
				jointSIDList.push_back(node_sid);
				break;
			}
		}
	}
}

float* SkinLoader::loadWeights(int& count)
{
	pugi::xpath_node inputNode = skinningData.node().child("vertex_weights");
	std::string weightsDataId = std::string(GetChildWithAttribute(inputNode, "input", "semantic", "WEIGHT").node().attribute("source").value()).substr(1);
	pugi::xpath_node weightsNode = GetChildWithAttribute(skinningData, "source", "id", weightsDataId).node().child("float_array");


	std::vector<std::string> rawData = StringSplit(weightsNode.node().text().as_string(), " ");
	count = rawData.size();
	float* weights = new float[count];
	for (int i = 0; i < count; i++)
	{
		weights[i] = std::stof(rawData[i]);
	}
	return weights;
}

int* SkinLoader::getEffectiveJointsCounts(int& count, pugi::xpath_node weightsDataNode)
{
	std::vector<std::string> rawData = StringSplit(weightsDataNode.node().child("vcount").text().as_string(), " ");
	count = rawData.size();
	int* counts = new int[count];
	for (int i = 0; i < count; i++)
	{
		counts[i] = std::stoi(rawData[i]);
	}
	return counts;
}

std::vector<VertexSkinData> SkinLoader::getSkinData(pugi::xpath_node weightsDataNode, int* counts, int countJoints, float* weights, int countWeights)
{
	std::vector<std::string> rawData = StringSplit(std::string(weightsDataNode.node().child("v").text().as_string()), " ");
	std::vector<VertexSkinData> skinningData;
	int pointer = 0;

	for (int i = 0; i < countJoints; i++)
	{
		int count = counts[i];
		VertexSkinData skinData = VertexSkinData();

		for (int i = 0; i < count; i++)
		{
			int jointId = std::stoi(rawData[pointer++]);
			int weightId = std::stoi(rawData[pointer++]);

			skinData.addJointEffect(jointId, weights[weightId]);
		}

		skinData.limitJointNumber(maxWeights);
		skinningData.push_back(skinData);
	}
	return skinningData;
}

std::vector<Matrix4> SkinLoader::loadBindPoses()
{
	pugi::xpath_node jointsDataNode = skinningData.node().child("joints");

	std::string bindPosesId = std::string(GetChildWithAttribute(jointsDataNode, "input", "semantic", "INV_BIND_MATRIX").node().attribute("source").value()).substr(1);

	pugi::xpath_node sourceFloatArray = GetChildWithAttribute(skinningData, "source", "id", bindPosesId).node().child("float_array");

	std::string floatDataStr = std::string(sourceFloatArray.node().text().as_string());
	
	std::vector<std::string> floatsData;

	Split(floatDataStr, " ", floatsData, true);


	int matriciesCount = floatsData.size() / 16;

	std::vector<Matrix4> bindPoses = std::vector<Matrix4>();
	
	for (int i = 0; i < matriciesCount; i++)
	{
		Matrix4 bindPose = ConvertDataToMatrix(floatsData, i);

		bindPose.Transpose();

		//if (i == 0)
		//{
		//	//because in Blender z is up, but in our game y is up.

		//	bindPose = AnimatedModelLoader::CORRECTION * bindPose;
		//}

		bindPoses.push_back(bindPose);
	}

	return bindPoses;
}