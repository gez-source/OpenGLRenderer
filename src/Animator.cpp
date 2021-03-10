#include "Animator.h"
#include "TimeManager.h"
#include <stack>

void Animator::doAnimation(Animation* animation)
{
	this->animationTime = 0;
	this->currentAnimation = animation;
}

std::map<std::string, Matrix4> Animator::GetDefaultPose()
{
	std::map<std::string, Matrix4> currentPose = std::map<std::string, Matrix4>();

	std::vector<KeyFrame*>* allFrames = currentAnimation->getKeyFrames();
	
	KeyFrame* firstFrame = allFrames->at(0);

	std::map<std::string, JointTransform*>* keyframeJointMap = firstFrame->getJointKeyFrames();

	for (auto const& keysmap : *keyframeJointMap)
	{
		std::string jointName = keysmap.first;
		JointTransform* currentTransform = keysmap.second;
		Matrix4 localTransform = currentTransform->getLocalTransform();

		currentPose.insert(std::pair<std::string, Matrix4>(jointName, localTransform));
	}

	return currentPose;
}

void Animator::update()
{
	if (currentAnimation != nullptr && currentAnimation->getKeyFrames()->size() > 0)
	{
		increaseAnimationTime();

		std::map<std::string, Matrix4> currentPose;

		//currentPose = GetDefaultPose();
		currentPose = calculateCurrentAnimationPose();

		Joint* root = entity->getRootJoint();

		if (root != nullptr)
		{
			Matrix4 currentTransform = Matrix4::GetIdentity();

			applyPoseToJoints(currentPose, root, currentTransform);
		}
	}
}

void Animator::increaseAnimationTime()
{
	animationTime += TimeManager::GetFrameTime();

	if (animationTime > currentAnimation->getLength()) 
	{
		animationTime = std::fmod(animationTime, currentAnimation->getLength());
	}
}

std::map<std::string, Matrix4> Animator::calculateCurrentAnimationPose()
{
	std::vector<KeyFrame*> frames = getPreviousAndNextFrames();

	float progression = calculateProgression(frames.at(0), frames.at(1));

	return interpolatePoses(frames.at(0), frames.at(1), progression);
}

void Animator::applyPoseToJoints(std::map<std::string, Matrix4> currentPose, Joint* rootJoint, Matrix4 parentTransform)
{
	Matrix4 currentLocalTransform;
	std::map<std::string, Matrix4>::iterator it = currentPose.find(rootJoint->id);
	if (it == currentPose.end())
	{
		it = currentPose.find(rootJoint->name);

		if (it == currentPose.end())
		{
			// There was no keyframe pose made so this model doesn't move from its bind pose.
			currentLocalTransform = rootJoint->getLocalBindTransform(); // Keeps the model in the bind pose
		}
		else
		{
			currentLocalTransform = currentPose.at(rootJoint->name);
		}
	}
	else 
	{
		currentLocalTransform = currentPose.at(rootJoint->id);
	}

	Matrix4 worldTransform = parentTransform * currentLocalTransform;

	Matrix4 inverseBindpose = rootJoint->getInverseBindTransform();

	// Combining the two should by default yield the identity matrix that is apply ZERO offset. 
	// inverseBindpose should be equal to worldTransform.Inverse().
	Matrix4 offsetMatrix = worldTransform * inverseBindpose;

	// The offsetMatrix is calculated by taking the desired model-space transform of
	// the joint and multiplying it by the inverse of the starting model-space
	// transform of the joint.

	//offsetMatrix = Matrix4::GetIdentity();

	rootJoint->setAnimationTransform(offsetMatrix); // animation should be in its bind pose when this is equal to the identity matrix.

	for (Joint* childJoint : rootJoint->children)
	{
		applyPoseToJoints(currentPose, childJoint, worldTransform);
	}
}

std::vector<KeyFrame*> Animator::getPreviousAndNextFrames()
{
	std::vector<KeyFrame*>* allFrames = currentAnimation->getKeyFrames();
	KeyFrame* previousFrame = allFrames->at(0);
	KeyFrame* nextFrame = allFrames->at(0);
	for (int i = 1; i < allFrames->size(); i++)
	{
		nextFrame = allFrames->at(i);
		if (nextFrame->getTimeStamp() > animationTime)
		{
			break;
		}
		previousFrame = allFrames->at(i);
	}

	std::vector<KeyFrame*> result = std::vector<KeyFrame*>();
	result.push_back(previousFrame);
	result.push_back(nextFrame);

	return result;
}

float Animator::calculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame)
{
	float totalTime = nextFrame->getTimeStamp() - previousFrame->getTimeStamp();
	float currentTime = animationTime - previousFrame->getTimeStamp();
	return currentTime / totalTime;
}

std::map<std::string, Matrix4> Animator::interpolatePoses(KeyFrame* previousFrame, KeyFrame* nextFrame, float progression)
{
	std::map<std::string, Matrix4> currentPose = std::map<std::string, Matrix4>();
	std::map<std::string, JointTransform*>* prevKeyframeMap = previousFrame->getJointKeyFrames();
	std::map<std::string, JointTransform*>* nextKeyframeMap = nextFrame->getJointKeyFrames();

	for (auto const& keysmap : *prevKeyframeMap)
	{
		std::string jointName = keysmap.first;

		JointTransform* previousTransform = prevKeyframeMap->at(jointName);
		JointTransform* nextTransform = nextKeyframeMap->at(jointName);

		JointTransform currentTransform = JointTransform::interpolate(*previousTransform, *nextTransform, progression);

		currentPose.insert(std::pair<std::string, Matrix4>(jointName, currentTransform.getLocalTransform()));
	}

	return currentPose;
}