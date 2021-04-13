// Octree Data Structure
// © Gerallt 2017 - 2020.

#ifndef OCTREE_H
#define OCTREE_H

class AABB;
class Octree;
class Transform;
class MeshBufferVAO;

#include <stack>
#include "AABB.h"
#include "MeshBufferVAO.h"

class Octree
{
public:
	/// <summary>
	/// The bounding box of the area.
	/// </summary>
	AABB* BBox;

	Octree* TopFrontRight;
	Octree* TopBackRight;
	Octree* TopBackLeft;
	Octree* TopFrontLeft;
	Octree* BottomFrontRight;
	Octree* BottomBackRight;
	Octree* BottomBackLeft;
	Octree* BottomFrontLeft;


	Transform* transform;
	Octree* parent;

	/// <summary>
	/// Number of objects that are allowed per area, before the area splits into quarters.
	/// Keeping the capacity value low results in a more balanced tree.
	/// </summary>
	int CapacityThreshold = 5;

	/// <summary>
	/// The objects in the current area.
	/// </summary>
	std::vector<MeshBufferVAO*>* sceneObjects = new std::vector<MeshBufferVAO*>();

	/// <summary>
	/// Check if the current Octree is empty.
	/// </summary>
	/// <returns>
	/// Returns true if the tree is empty.
	/// </returns>
	bool IsEmpty();

	bool Add(Octree* octTree);

	/// <summary>
	/// Inserts a mesh bufer VAO into the tree.
	/// </summary>
	/// <returns>
	/// Returns true if the MeshBufferVAO could be inserted in the tree, 
	/// otherwise false if the MeshBufferVAO could not be added. 
	/// </returns>
	bool Add(MeshBufferVAO* meshBufferVAO);

	/// <summary>
	/// Subdivide the current Octree 
	/// </summary>
	void Subdivide();

	/// <summary>
	/// Find the nearest MeshBufferVAO objects in the specified area.
	/// </summary>
	/// <param name="area">
	/// A bounding box that encapsulates the area.
	/// </param>
	/// <returns>
	/// A list of objects in the area. (For now just test points)
	/// </returns>
	void FindNearestObjects(AABB area, std::vector<MeshBufferVAO*>& nearestList);
};

#endif
