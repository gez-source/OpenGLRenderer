//// QuadTree Data Structure
//// © Gerallt 2017 - 2020.
//
//#ifndef QUADTREE_H
//#define QUADTREE_H
//
//#include <stack>
//
//class AABB;
//class Transform;
//class QuadTree;
//class HE_Face;
//
//#include "HE_Face.h"
//#include "AABB.h"
//
//class QuadTree
//{
//public:
//	/// <summary>
//	/// The bounding box of the area.
//	/// </summary>
//	AABB* BBox;
//
//	QuadTree* TopLeft;
//	QuadTree* TopRight;
//	QuadTree* BottomLeft;
//	QuadTree* BottomRight;
//
//	Transform* transform;
//	QuadTree* parent;
//
//	/// <summary>
//	/// Number of objects that are allowed per area, before the area splits into quarters.
//	/// Keeping the capacity value low results in a more balanced tree.
//	/// </summary>
//	int CapacityThreshold = 5;
//
//	/// <summary>
//	/// The objects in the current area.
//	/// </summary>
//	std::vector<HE_Face*>* Polygons = new std::vector<HE_Face*>();
//
//	/// <summary>
//	/// Check if the current QuadTree is empty.
//	/// </summary>
//	/// <returns>
//	/// Returns true if the tree is empty.
//	/// </returns>
//	bool IsEmpty();
//
//	bool Add(QuadTree* quadTree);
//
//	/// <summary>
//	/// Insert a point into the tree.
//	/// </summary>
//	/// <returns>
//	/// Returns true if the point could be inserted in the tree, 
//	/// otherwise false if the object could not be added. 
//	/// </returns>
//	bool Add(HE_Face* polygon);
//
//	void Subdivide();
//
//	/// <summary>
//	/// Find the nearest objects in the specified area.
//	/// </summary>
//	/// <param name="area">
//	/// A bounding box that encapsulates the area.
//	/// </param>
//	/// <returns>
//	/// A list of objects in the area. (For now just test points)
//	/// </returns>
//	std::vector<HE_Face*>* FindNearest(AABB* area);
//};
//
//#endif
