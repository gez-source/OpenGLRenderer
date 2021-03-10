// Octree Data Structure
// © Gerallt 2017 - 2020.

#ifndef OCTREE_H
#define OCTREE_H

class AABB;
class Octree;
class Transform;
class IndexedTriangleSet;

#include <stack>
#include "AABB.h"
#include "IndexedTriangleSet.h"

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
	//std::vector<HE_Face*>* Polygons = new std::vector<HE_Face*>();

	std::vector< IndexedTriangleSet*>* sceneObjects = new std::vector< IndexedTriangleSet*>();

	/// <summary>
	/// Check if the current QuadTree is empty.
	/// </summary>
	/// <returns>
	/// Returns true if the tree is empty.
	/// </returns>
	bool IsEmpty();

	bool Add(Octree* octTree);

	/// <summary>
	/// Insert a point into the tree.
	/// </summary>
	/// <returns>
	/// Returns true if the point could be inserted in the tree, 
	/// otherwise false if the object could not be added. 
	/// </returns>
	//bool Add(HE_Face* polygon);

	void Subdivide();

	/// <summary>
	/// Find the nearest objects in the specified area.
	/// </summary>
	/// <param name="area">
	/// A bounding box that encapsulates the area.
	/// </param>
	/// <returns>
	/// A list of objects in the area. (For now just test points)
	/// </returns>
	//std::vector<HE_Face*>* FindNearest(AABB* area);

	void FindNearestObjects(AABB area, std::vector<IndexedTriangleSet*>& nearestList);
};

#endif
