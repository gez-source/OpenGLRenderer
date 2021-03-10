#include "AnimatedModelLoader.h"
#include "XMLColladaLoader.h"

Matrix4 AnimatedModelLoader::CORRECTION = Matrix4::GetIdentity();

AnimatedModel* AnimatedModelLoader::LoadModel(std::string modelFile)
{
	bool loadedData;
	bool loadedSkeletonData;
	bool loadedAnimData;

	CORRECTION = Matrix4::GetIdentity();
	//CORRECTION = CORRECTION.RotationYawPitchRoll(0, -90, 0);

	// Parse Collada.
	AnimatedModelData* entityData = ColladaLoader::loadColladaModel(modelFile, MAX_WEIGHTS, loadedData, loadedSkeletonData, loadedAnimData);

	// Load Model.
	Vao* model = createVao(entityData->getMeshData());
	SkeletonData skeletonData = entityData->getJointsData();
	Joint* headJoint = nullptr;
	int jointCount = 0;
	if(loadedSkeletonData)
	{
		headJoint = createJoints(skeletonData.headJoint);
		jointCount = skeletonData.jointCount;
	}
	
	// Load Animation.
	std::vector<KeyFrame*>* frames = new std::vector<KeyFrame*>();
	float lengthSeconds = 0;

	if (loadedAnimData)
	{
		AnimationData* animationData = entityData->getAnimationData();
		lengthSeconds = animationData->lengthSeconds;

		for (int i = 0; i < animationData->keyFrames->size(); i++)
		{
			frames->push_back(createKeyFrame(animationData->keyFrames->at(i)));
		}
	}

	Animation* animation = new Animation(lengthSeconds, frames);

	// Combine into one object.
	AnimatedModel* animatedModel = new AnimatedModel(model, headJoint, jointCount, entityData, animation);

	return animatedModel;
}

Joint* AnimatedModelLoader::createJoints(JointData data)
{
	Joint* joint = new Joint(data.index, data.id, data.name, data.sid, data.bindLocalTransform, data.inverseBindPoseTransform);
	for (JointData child : data.children) 
	{
		joint->addChild(createJoints(child));
	}
	return joint;
}

Vao* AnimatedModelLoader::createVao(MeshData* data)
{
	if (data == nullptr) return nullptr;
	Vao* vao = Vao::create();
	vao->bind(); 
	vao->createIndexBuffer(data->getIndices());
	vao->createAttribute(0, data->getVertices(), 3);
	vao->createAttribute(1, data->getTextureCoords(), 2);
	vao->createAttribute(2, data->getNormals(), 3);
	vao->createIntAttribute(3, data->getJointIds(), 3);
	vao->createAttribute(4, data->getVertexWeights(), 3);
	vao->createAttribute(5, data->getVertexTangents(), 3);
	vao->createAttribute(6, data->getVertexBiTangents(), 3);
	vao->unbind();
	return vao;
}

KeyFrame* AnimatedModelLoader::createKeyFrame(KeyFrameData* data)
{
	std::map<std::string, JointTransform*>* mapPose = new std::map<std::string, JointTransform*>();
	for (JointTransformData* jointData : *data->jointTransforms)
	{
		JointTransform* jointTransform = createTransform(jointData);
		mapPose->insert(std::pair<std::string, JointTransform*>(jointData->jointNameId, jointTransform));
	}
	return new KeyFrame(data->time, mapPose);
}

JointTransform* AnimatedModelLoader::createTransform(JointTransformData* data)
{
	Matrix4 mat = data->jointLocalTransform;
	Vector3 translation = mat.GetTranslation();
	Quaternion rotation = Quaternion::FromMatrix(mat);

	return new JointTransform(translation, rotation);
}