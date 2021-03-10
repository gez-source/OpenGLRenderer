#ifndef ANIMATED_MODEL_LOADER_H
#define ANIMATED_MODEL_LOADER_H

#include <string>
#include "AnimatedModel.h"
#include "AnimatedModelData.h"
#include "Joint.h"
#include "Vao.h"

const int MAX_WEIGHTS = 3;

class AnimatedModelLoader 
{
public:
	static Matrix4 CORRECTION;

	/**
	 * Creates an AnimatedEntity from the data in an entity file. It loads up
	 * the collada model data, stores the extracted data in a VAO, sets up the
	 * joint heirarchy, and loads up the entity's texture.
	 *
	 * @param entityFile
	 *            - the file containing the data for the entity.
	 * @return The animated entity (no animation applied though)
	 */
	static AnimatedModel* LoadModel(std::string modelFile);

private:

	/**
	 * Constructs the joint-hierarchy skeleton from the data extracted from the
	 * collada file.
	 *
	 * @param data
	 *            - the joints data from the collada file for the head joint.
	 * @return The created joint, with all its descendants added.
	 */
	static Joint* createJoints(JointData data);

	/**
	 * Stores the mesh data in a VAO.
	 *
	 * @param data
	 *            - all the data about the mesh that needs to be stored in the
	 *            VAO.
	 * @return The VAO containing all the mesh data for the model.
	 */
	static Vao* createVao(MeshData* data);

	/**
	 * Creates a keyframe from the data extracted from the collada file.
	 *
	 * @param data
	 *            - the data about the keyframe that was extracted from the
	 *            collada file.
	 * @return The keyframe.
	 */
	static KeyFrame* createKeyFrame(KeyFrameData* data);

	/**
	 * Creates a joint transform from the data extracted from the collada file.
	 *
	 * @param data
	 *            - the data from the collada file.
	 * @return The joint transform.
	 */
	static JointTransform* createTransform(JointTransformData* data);
};

#endif