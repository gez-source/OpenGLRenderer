#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <vector>
#include "Matrix4.h"

/**
 *
 * Represents a joint in a "skeleton". It contains the index of the joint which
 * determines where in the vertex shader uniform array the joint matrix for this
 * joint is loaded up to. It also contains the name of the bone, and a list of
 * all the child joints.
 *
 * The "animatedTransform" matrix is the joint transform that I keep referring
 * to in the tutorial. This is the transform that gets loaded up to the vertex
 * shader and is used to transform vertices. It is a model-space transform that
 * transforms the joint from it's bind (original position, no animation applied)
 * position to it's current position in the current pose. Changing this
 * transform changes the position/rotation of the joint in the animated entity.
 *
 * The two other matrices are transforms that are required to calculate the
 * "animatedTransform" in the {@link Animator} class. It also has the local bind
 * transform which is the original (no pose/animation applied) transform of the
 * joint relative to the parent joint (in bone-space).
 *
 * The "localBindTransform" is the original (bind) transform of the joint
 * relative to its parent (in bone-space). The inverseBindTransform is that bind
 * transform in model-space, but inversed.
 *
 * @author Karl
 *
 */
class Joint 
{
private:
	Matrix4 animatedTransform = Matrix4::GetIdentity();
	Matrix4 localBindTransform = Matrix4::GetIdentity();
	Matrix4 inverseBindTransform = Matrix4::GetIdentity();
	Matrix4 inverseBindTransformCalc = Matrix4::GetIdentity();

public:
	int index; // ID
	std::string id;
	std::string name;
	std::string sid;
	std::vector<Joint*> children;
	Joint* parent;

	Joint() : index(-1), parent(nullptr)
	{

	}

	~Joint() 
	{
		if (parent != nullptr)
		{
			delete parent;
		}

		for (Joint* child : children)
		{
			delete child;
		}
	}

	/**
	 * @param index
	 *            - the joint's index (ID).
	 * @param name
	 *            - the name of the joint. This is how the joint is named in the
	 *            collada file, and so is used to identify which joint a joint
	 *            transform in an animation keyframe refers to.
	 * @param bindLocalTransform
	 *            - the bone-space transform of the joint in the bind position.
	 */
	Joint(int index, std::string id, std::string name, std::string sid, Matrix4 bindLocalTransform, Matrix4 inverseBindTransform)
	{
		this->index = index;
		this->id = id;
		this->name = name;
		this->sid = sid;
		this->localBindTransform = bindLocalTransform;
		this->inverseBindTransform = inverseBindTransform;
		this->parent = nullptr;
	}

	/**
	 * Adds a child joint to this joint. Used during the creation of the joint
	 * hierarchy. Joints can have multiple children, which is why they are
	 * stored in a list (e.g. a "hand" joint may have multiple "finger" children
	 * joints).
	 *
	 * @param child
	 *            - the new child joint of this joint.
	 */
	void addChild(Joint* child);

	Matrix4 getLocalBindTransform();

	/**
	 * The animated transform is the transform that gets loaded up to the shader
	 * and is used to deform the vertices of the "skin". It represents the
	 * transformation from the joint's bind position (original position in
	 * model-space) to the joint's desired animation pose (also in model-space).
	 * This matrix is calculated by taking the desired model-space transform of
	 * the joint and multiplying it by the inverse of the starting model-space
	 * transform of the joint.
	 *
	 * @return The transformation matrix of the joint which is used to deform
	 *         associated vertices of the skin in the shaders.
	 */
	Matrix4 getAnimatedTransform();

	/**
	 * This method allows those all important "joint transforms" (as I referred
	 * to them in the tutorial) to be set by the animator. This is used to put
	 * the joints of the animated model in a certain pose.
	 *
	 * @param animationTransform - the new joint transform.
	 */
	void setAnimationTransform(Matrix4 animationTransform);

	/**
	 * This returns the inverted model-space bind transform. The bind transform
	 * is the original model-space transform of the joint (when no animation is
	 * applied). This returns the inverse of that, which is used to calculate
	 * the animated transform matrix which gets used to transform vertices in
	 * the shader.
	 *
	 * @return The inverse of the joint's bind transform (in model-space).
	 */
	Matrix4 getInverseBindTransform();

	/**
	 * This is called during set-up, after the joints hierarchy has been
	 * created. This calculates the model-space bind transform of this joint
	 * like so: </br>
	 * </br>
	 * {@code bindTransform = parentBindTransform * localBindTransform}</br>
	 * </br>
	 * where "bindTransform" is the model-space bind transform of this joint,
	 * "parentBindTransform" is the model-space bind transform of the parent
	 * joint, and "localBindTransform" is the bone-space bind transform of this
	 * joint. It then calculates and stores the inverse of this model-space bind
	 * transform, for use when calculating the final animation transform each
	 * frame. It then recursively calls the method for all of the children
	 * joints, so that they too calculate and store their inverse bind-pose
	 * transform.
	 *
	 * @param parentBindTransform
	 *            - the model-space bind transform of the parent joint.
	 */
	void calcInverseBindTransform(Matrix4 parentBindTransform);

};

#endif
