#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H
#include <GL/glew.h>

/**
 * Useful class for updating OpenGL state, such as alpha blending, depth testing, etc.
 *
 * @author Karl
 *
 */
class OpenGlUtils 
{
public:
	static void Antialias(bool enable);
	static void EnableAlphaBlending();
	static void EnableAdditiveBlending();
	static void DisableBlending();
	static void EnableDepthTesting(bool enable);
	static void CullBackFaces(bool cull);
	static void GoWireframe(bool goWireframe);

};

#endif