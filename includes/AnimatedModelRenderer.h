#ifndef ANIMATED_MODEL_RENDERER_H
#define ANIMATED_MODEL_RENDERER_H

#include <GL/glew.h>
#include "Vector3.h"
#include "AnimatedModelShader.h"
#include "AnimatedModel.h"
#include "Transform.h"
#include "OpenGlUtils.h"

/**
 *
 * This class deals with rendering an animated entity. Nothing particularly new
 * here. The only exciting part is that the joint transforms get loaded up to
 * the shader in a uniform array.
 *
 * @author Karl
 *
 */
class AnimatedModelRenderer 
{
private:
	AnimatedModelShader* shader;

public:

	/**
	 * Initializes the shader program used for rendering animated models.
	 */
	AnimatedModelRenderer();

	/**
	 * Deletes the shader program when the game closes.
	 */
	~AnimatedModelRenderer();

	/**
	 * Renders an animated entity. The main thing to note here is that all the
	 * joint transforms are loaded up to the shader to a uniform array. Also 5
	 * attributes of the VAO are enabled before rendering, to include joint
	 * indices and weights.
	 *
	 * @param entity
	 *            - the animated entity to be rendered.
	 * @param camera
	 *            - the camera used to render the entity.
	 * @param lightDir
	 *            - the direction of the light in the scene.
	 */
	void render(AnimatedModel* entity, Transform camera, Matrix4 projection, Matrix4 modelview, Vector3 lightDir);

	/**
	 * Starts the shader program and loads up the projection view matrix, as
	 * well as the light direction. Enables and disables a few settings which
	 * should be pretty self-explanatory.
	 *
	 * @param camera
	 *            - the camera being used.
	 * @param lightDir
	 *            - the direction of the light in the scene.
	 */
	void prepare(Transform camera, Matrix4 projection, Matrix4 modelview, Vector3 lightDir);

private:
	/**
	 * Stops the shader program after rendering the entity.
	 */
	void finish();

};

#endif