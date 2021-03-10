#ifndef JOINT_DATA_H
#define JOINT_DATA_H

#include <string>
#include <vector>
#include "Matrix4.h"

/**
 * Contains the extracted data for a single joint in the model. This stores the
 * joint's index, name, and local bind transform.
 *
 * @author Karl
 *
 */
class JointData 
{
public:
	int index;
	std::string id;
	std::string name;
	std::string sid;
	Matrix4 bindLocalTransform;
	Matrix4 inverseBindPoseTransform;

	std::vector<JointData> children = std::vector<JointData>();

	JointData() : index(0)
	{

	}

	JointData(int index, std::string id, std::string name, std::string sid, Matrix4 bindLocalTransform, Matrix4 inverseBindPoseTransform)
	{
		this->index = index;
		this->id = id;
		this->name = name;
		this->sid = sid;
		this->bindLocalTransform = bindLocalTransform;
		this->inverseBindPoseTransform = inverseBindPoseTransform;
	}

	void addChild(JointData child);

};


#endif