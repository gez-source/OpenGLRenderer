#include "JointData.h"

void JointData::addChild(JointData child)
{
	children.push_back(child);
}