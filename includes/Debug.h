#ifndef DEBUG_H
#define DEBUG_H

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <queue>

#include "Vector3.h"
#include "ColourRGBA.h"
#include "Transform.h"
#include "Matrix4.h"
#include "ShaderProgram.h"

struct LineRenderable;
typedef struct LineRenderable LineRenderable_T;

class Debug
{
private:
	static std::queue<LineRenderable_T> renderQueue;
	static ShaderProgram* program;
public:
	static void DrawLine(Vector3 start, Vector3 end, ColourRGBA colour);
	static void DrawLines_Init();
	static void DrawLines_RenderDispatch(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview);
};

#endif