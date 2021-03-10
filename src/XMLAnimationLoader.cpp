#include "XMLAnimationLoader.h"
#include "AnimatedModelLoader.h"
#include <iostream>

AnimationLoader::AnimationLoader(pugi::xpath_node animationData, pugi::xpath_node jointHierarchy)
{
	this->animationData = animationData;
	this->jointHierarchy = jointHierarchy;
}

AnimationData* AnimationLoader::extractAnimation() 
{
	checkNodeHierarchy();

	std::string rootNode = findRootJointName();

	std::vector<float> times = getKeyTimes();

	float duration = times[times.size() - 1];

	std::vector<KeyFrameData*>* keyFrames = initKeyFrames(times);

	auto animationNodes = animationData.node().children("animation");
	for (pugi::xpath_node jointNode : animationNodes)
	{
		loadJointTransforms(keyFrames, jointNode, rootNode);
	}
	return new AnimationData(duration, keyFrames);
}

std::vector<float> AnimationLoader::getKeyTimes()
{
	pugi::xpath_node timeData = animationData.node().child("animation").child("source").child("float_array");
	std::vector<std::string> rawTimes = StringSplit(std::string(timeData.node().text().as_string()), " ");
	int timesCount = rawTimes.size();
	std::vector<float> times = std::vector<float>(timesCount);
	for (int i = 0; i < timesCount; i++)
	{
		times[i] = std::stof(rawTimes[i]);
	}
	return times;
}

std::vector<KeyFrameData*>* AnimationLoader::initKeyFrames(std::vector<float> times)
{
	std::vector<KeyFrameData*>* frames = new std::vector<KeyFrameData*>(times.size());
	for (int i = 0; i < frames->size(); i++) {
		frames->at(i) = new KeyFrameData(times[i]);
	}
	return frames;
}

void AnimationLoader::loadJointTransforms(std::vector<KeyFrameData*>* frames, pugi::xpath_node jointData, std::string rootNodeId)
{
	std::string jointNameId = getJointName(jointData);
	std::string dataId = getDataId(jointData);

	pugi::xpath_node transformData = GetChildWithAttribute(jointData, "source", "id", dataId);
	std::string rawData = std::string(transformData.node().child("float_array").text().as_string());

	processTransforms(jointNameId, rawData, frames, jointNameId == rootNodeId);
}

std::string AnimationLoader::getDataId(pugi::xpath_node jointData)
{
	std::string animID = std::string(jointData.node().attribute("id").value());
	pugi::xpath_node sampler = jointData.node().child("sampler");
	std::string samplerID = std::string(sampler.node().attribute("id").value());
	pugi::xpath_node node = GetChildWithAttribute(sampler, "input", "semantic", "OUTPUT");
	const pugi::char_t* sourceId = node.node().attribute("source").value();
	std::string dataId = std::string(sourceId).substr(1);
	return dataId;
}

std::string AnimationLoader::getJointName(pugi::xpath_node jointData)
{
	pugi::xpath_node channelNode = jointData.node().child("channel");
	std::string data = std::string(channelNode.node().attribute("target").value());
	std::string jointName = StringSplit(data, "/")[0];
	return jointName;
}

void AnimationLoader::processTransforms(std::string jointName, std::string rawData, std::vector<KeyFrameData*>* keyFrames, bool root)
{
	std::vector<std::string> data;
	
	Split(rawData, " ", data, true);

	for (int i = 0; i < keyFrames->size(); i++)
	{
		Matrix4 transform = ConvertDataToMatrix(data, i);

		// Find the transpose: Convert from a Collada row-major order to column-major order.
		transform.Transpose();

		if (root)
		{
			//because up axis in Blender is different to up axis in game
			transform = AnimatedModelLoader::CORRECTION * transform;
		}

		JointTransformData* jointTransformData = new JointTransformData(jointName, transform);
		keyFrames->at(i)->addJointTransform(jointTransformData);
	}
}

std::string AnimationLoader::findRootJointName()
{
	pugi::xpath_node visualScene = jointHierarchy.node().child("visual_scene");
	auto it = visualScene.node().children("node");

	// Check for Armature Node, Node id=Armature.
	pugi::xpath_node skeleton = GetChildWithAttribute(visualScene, "node", "id", "Armature");

	// Find ID of first node
	if (skeleton.node().empty())
	{
		skeleton = visualScene.node().child("node");

		return std::string(skeleton.node().attribute("id").value());
	}
	else 
	{
		return std::string(skeleton.node().child("node").attribute("id").value());
	}
}

void AnimationLoader::checkNodeHierarchy() 
{
	pugi::xpath_node visualScene = jointHierarchy.node().child("visual_scene");
	auto it = visualScene.node().children("node");
	int countNodes = std::distance(it.begin(), it.end());

	if (countNodes == 0)
	{
		std::cout << "There are no nodes present under visual_scene." << std::endl;
	}
	else if (countNodes > 1)
	{
		std::cout << "There are many root nodes present under visual_scene. Consider only having one. Choosing the first node." << std::endl;
	}
}