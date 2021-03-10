#include "XMLSkeletonLoader.h"
#include "AnimatedModelLoader.h"

SkeletonLoader::SkeletonLoader(pugi::xpath_node jointHierarchy, std::vector<std::string> boneOrder, std::vector<Matrix4> bindPoses)
{
	this->boneOrder = boneOrder;
	this->bindPoses = bindPoses;

	pugi::xpath_node visualScene = jointHierarchy.node().child("visual_scene");

	// Check for Armature Node, Node id=Armature.
	pugi::xpath_node skeleton = GetChildWithAttribute(visualScene, "node", "id", "Armature");

	if (skeleton.node().empty())
	{
		// Just find the first node (hopefully it is the root node).
		this->armatureData = visualScene.node().child("node");
	}
	else 
	{
		// We have the Armature as the root node.
		this->armatureData = skeleton;
	}
}

SkeletonData SkeletonLoader::extractBoneData(bool& loaded)
{
	JointData headJoint;
	pugi::xpath_node headNode = armatureData.node().child("node");

	if (!headNode.node().empty()) 
	{
		headJoint = loadJointData(headNode, true);

		loaded = true;
	}
	
	return SkeletonData(jointCount, headJoint);
}

JointData SkeletonLoader::loadJointData(pugi::xpath_node jointNode, bool isRoot)
{
	JointData joint = extractMainJointData(jointNode, isRoot);

	for (pugi::xpath_node childNode : jointNode.node().children("node"))
	{
		joint.addChild(loadJointData(childNode, false));
	}

	return joint;
}

JointData SkeletonLoader::extractMainJointData(pugi::xpath_node jointNode, bool isRoot)
{
	std::string sid = std::string(jointNode.node().attribute("sid").value());
	std::string name = std::string(jointNode.node().attribute("name").value());
	std::string id = std::string(jointNode.node().attribute("id").value()); 
	// blender changes the id but it is best to stick with the id attribute

	int index = -1;
	auto it = std::find(boneOrder.begin(), boneOrder.end(), id);
	if (it != boneOrder.end())
	{
		index = std::distance(boneOrder.begin(), it);
	}

	std::string matrixData = std::string(jointNode.node().child("matrix").text().as_string());
	Matrix4 localBindTransform; 

	localBindTransform = ConvertDataToMatrix(matrixData);

	// Find the transpose: Convert from a Collada row-major order to column-major order.
	localBindTransform.Transpose(); 
	
	if (isRoot)
	{
		//because in Blender z is up, but in our game y is up.
		localBindTransform = AnimatedModelLoader::CORRECTION * localBindTransform;
	}

	Matrix4 inverseBindPoseTransform = bindPoses[index];

	jointCount++;
	return JointData(index, id, name, sid, localBindTransform, inverseBindPoseTransform);
}