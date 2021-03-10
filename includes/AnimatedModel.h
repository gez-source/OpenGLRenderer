#ifndef ANIMATED_MODEL_H
#define ANIMATED_MODEL_H

#include <vector>
#include "Vao.h"
#include "Texture.h"
#include "Joint.h"
#include "Animator.h"
#include "Animation.h"
#include "Matrix4.h"
#include "AnimatedModelData.h"

class AnimatedModel;
class Animation;
class Animator;
class Texture;
class Joint;
class Vao;

/**
 *
 * This class represents an entity in the world that can be animated. It
 * contains the model's VAO which contains the mesh data, the texture, and the
 * root joint of the joint hierarchy, or "skeleton". It also holds an int which
 * represents the number of joints that the model's skeleton contains, and has
 * its own {@link Animator} instance which can be used to apply animations to
 * this entity.
 *
 * @author Karl
 *
 */
class AnimatedModel
{
private:
	// skin
	Vao* model;
	Texture* texture;

	// skeleton
	Joint* rootJoint;
	int jointCount;

	AnimatedModelData* entityData;
	Animation* animation;

public:
	AnimatedModel() : jointCount(0), entityData(nullptr), rootJoint(nullptr), texture(nullptr), model(nullptr), animation(nullptr)
	{

	}

	/**
	 * Creates a new entity capable of animation. The inverse bind transform for
	 * all joints is calculated in this constructor. The bind transform is
	 * simply the original (no pose applied) transform of a joint in relation to
	 * the model's origin (model-space). The inverse bind transform is simply
	 * that but inverted.
	 *
	 * @param model
	 *            - the VAO containing the mesh data for this entity. This
	 *            includes vertex positions, normals, texture coords, IDs of
	 *            joints that affect each vertex, and their corresponding
	 *            weights.
	 * @param texture
	 *            - the diffuse texture for the entity.
	 * @param rootJoint
	 *            - the root joint of the joint hierarchy which makes up the
	 *            "skeleton" of the entity.
	 * @param jointCount
	 *            - the number of joints in the joint hierarchy (skeleton) for
	 *            this entity.
	 *
	 */
	AnimatedModel(Vao* model, Joint* rootJoint, int jointCount, AnimatedModelData* entityData, Animation* animation);

	/**
	 * Deletes the OpenGL objects associated with this entity, namely the model
	 * (VAO) and texture.
	 */
	~AnimatedModel();

	/**
	 * @return The VAO containing all the mesh data for this entity.
	 */
	Vao* getModel();

	/**
	 * @return The root joint of the joint hierarchy. This joint has no parent,
	 *         and every other joint in the skeleton is a descendant of this
	 *         joint.
	 */
	Joint* getRootJoint();

	Animation* getAnimation();

	  /**
	   * Gets an array of the all important model-space transforms of all the
	   * joints (with the current animation pose applied) in the entity. The
	   * joints are ordered in the array based on their joint index. The position
	   * of each joint's transform in the array is equal to the joint's index.
	   *
	   * @return The array of model-space transforms of the joints in the current
	   *         animation pose.
	   */
	std::vector<Matrix4> getJointTransforms();
};


#endif