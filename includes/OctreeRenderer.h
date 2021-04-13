// 1. Compress wavefront obj to octree structure.
//		Octree contains a list of IndexedTriangleSet scene objects for a given area
//		Each scene object has an IndexedTriangleSet and its AABB computed (by ITS) and world position defined.
//		A IndexedTriangleSet can be read from the wavefront obj importer.
// 2. Render octree structure; buffering and drawing polygons.
//		FindNearestObjects(AABB) is called to get a list of scene objects that can be rendered for the given area (defined by camera position).
//		Objects that are not initilised are initilised, then rendered at their specified world position.
// 3. Save octree structure into binary file.
// 4. Load octree structure from binary file.

#ifndef OCTREE_RENDERER_H
#define OCTREE_RENDERER_H

#include <string>

#include <GL/glew.h>		// Include GLEW and new version of GL on Windows.
#include <GLFW/glfw3.h>		// GLFW helper library.

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Octree.h"
#include "Transform.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "MeshBufferVAO.h"
#include "ShaderHelpers.h"

class OctreeRenderer
{
private:
	Octree* octreeInput;

	ShaderProgram* program = nullptr;
	GLuint depthTexture = 0;
	Matrix4 depthMVP;

public:
	Texture* lutTexture00;
	Texture* lutTexture01;
	Texture* lutTexture02;
	Texture* lutTexture03;
	Texture* lutTexture04;
public:
	OctreeRenderer();
	~OctreeRenderer();

	/// <summary>
	/// Compress Wavefront OBJ file into a octree data structure for fast lookup.
	/// </summary>
	/// <param name="meshBufferVAO">
	///	The loaded MeshBufferVAO 3D model.
	/// </param>
	/// <param name="octree">
	/// The resultant octree output.
	/// </param>
	void AddOBJToOctree(MeshBufferVAO* meshBufferVAO);

	// TODO: function to append OBJ to Octree at specified world position

	/// <summary>
	/// Saves the specified octree to a binary file.
	/// </summary>
	/// <param name="fileName">
	/// The specified filename of the output file.
	/// </param>
	/// <param name="octree">
	/// The specified octree to save.
	/// </param>
	/// <returns>
	/// True, if saved sucessfully. Otherwise false if could not save.
	/// </returns>
	bool SaveToBinaryFile(std::string fileName, Octree* octree);

	/// <summary>
	/// Loads the specified octree from a binary file.
	/// </summary>
	/// <param name="fileName">
	/// The specified filename of the input file.
	/// </param>
	/// <param name="octree">
	/// The specified octree to load.
	/// </param>
	/// <returns>
	/// True, if loaded sucessfully. Otherwise false if could not load.
	/// </returns>
	bool LoadFromBinaryFile(std::string fileName, Octree* octree);

	void Initilise();
	void Render(sf::RenderWindow* window, Transform cameraTransform, Matrix4 projectionMatrix, Matrix4 modelview, Vector3 light_dir, Matrix4 lightModelView);
	void RenderAABB();
};

#endif