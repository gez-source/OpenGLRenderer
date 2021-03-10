#include "Octree.h"

/// <summary>
/// Check if the current QuadTree is empty.
/// </summary>
/// <returns>
/// Returns true if the tree is empty.
/// </returns>
bool Octree::IsEmpty()
{
	return TopBackLeft == nullptr && TopBackRight == nullptr && TopFrontLeft == nullptr && TopFrontRight == nullptr
		&& BottomFrontLeft == nullptr && BottomBackLeft == nullptr && BottomBackRight == nullptr && BottomFrontRight == nullptr;
}

bool Octree::Add(Octree* octTree)
{
	if (BBox == nullptr) throw "Bounding box must not be null. Just calculate the BBox for the root QuadTree and the decendants are calcualted automatically.";

	bool inserted = false;
	Octree* node;
	std::stack<Octree*> stack;
	stack.push(this);

	while (stack.size() > 0 && !inserted)
	{
		node = stack.top();
		stack.pop();

		// Ignore areas that the object (p) does not belong in.
		if (node->BBox->Intersects(octTree->BBox))
		{
			if (node->IsEmpty())
			{
				node->Subdivide();

				// Create a point in the tree where the existing quadtree belongs.

				node->TopBackLeft = octTree;
				inserted = true;
			}
			else
			{
				// Continue the search frontier.
				if (node->TopBackLeft != nullptr)
				{
					stack.push(node->TopBackLeft);
				}
				if (node->TopBackRight != nullptr)
				{
					stack.push(node->TopBackRight);
				}
				if (node->TopFrontLeft != nullptr)
				{
					stack.push(node->TopFrontLeft);
				}
				if (node->TopFrontRight != nullptr)
				{
					stack.push(node->TopFrontRight);
				}

				if (node->BottomBackLeft != nullptr)
				{
					stack.push(node->BottomBackLeft);
				}
				if (node->BottomBackRight != nullptr)
				{
					stack.push(node->BottomBackRight);
				}
				if (node->BottomFrontLeft != nullptr)
				{
					stack.push(node->BottomFrontLeft);
				}
				if (node->BottomFrontRight != nullptr)
				{
					stack.push(node->BottomFrontRight);
				}
			}
		}
	}

	return inserted;
}

///// <summary>
///// Insert a point into the tree.
///// </summary>
///// <returns>
///// Returns true if the point could be inserted in the tree, 
///// otherwise false if the object could not be added. 
///// </returns>
//bool Octree::Add(HE_Face* polygon)
//{
//	if (BBox == nullptr) throw "Bounding box must not be null. Just calculate the BBox for the root QuadTree and the decendants are calcualted automatically.";
//
//	bool inserted = false;
//	Octree* node;
//	std::stack<Octree*> stack;
//	stack.push(this);
//
//	while (stack.size() > 0 && !inserted)
//	{
//		node = stack.top();
//		stack.pop();
//
//		// Ignore areas that the object (p) does not belong in.
//		if (node->BBox->Intersects(polygon))
//		{
//			// Only add an object under this node if under capacity. This should create a balanced tree.
//			if (node->Polygons->size() < node->CapacityThreshold)
//			{
//				node->Polygons->push_back(polygon);
//				inserted = true;
//			}
//			else
//			{
//				// Otherwise, subdivide and then add the point to whichever node will accept it.
//				if (node->IsEmpty())
//				{
//					node->Subdivide();
//				}
//
//				// Continue the search frontier.
//				if (node->TopBackLeft != nullptr)
//				{
//					stack.push(node->TopBackLeft);
//				}
//				if (node->TopBackRight != nullptr)
//				{
//					stack.push(node->TopBackRight);
//				}
//				if (node->TopFrontLeft != nullptr)
//				{
//					stack.push(node->TopFrontLeft);
//				}
//				if (node->TopFrontRight != nullptr)
//				{
//					stack.push(node->TopFrontRight);
//				}
//
//				if (node->BottomBackLeft != nullptr)
//				{
//					stack.push(node->BottomBackLeft);
//				}
//				if (node->BottomBackRight != nullptr)
//				{
//					stack.push(node->BottomBackRight);
//				}
//				if (node->BottomFrontLeft != nullptr)
//				{
//					stack.push(node->BottomFrontLeft);
//				}
//				if (node->BottomFrontRight != nullptr)
//				{
//					stack.push(node->BottomFrontRight);
//				}
//			}
//		}
//	}
//
//	return inserted;
//}

void Octree::Subdivide()
{
	if (BBox == nullptr) throw "Bounding box must not be null";

	// Given the current bounding box, divide the current half size and center by 2
	// and then use these new values to create 4 new quad children of equal area.

	float halfSize = BBox->HalfSize / 2.0f;

	float h = halfSize; // Easier to read.
	Vector3 c = BBox->Center;

	// Subdivide into 8 octants or 3 planes. (the octants are like 8 mini cubes stacked together)

	// Octant 0.
	TopFrontLeft = new Octree();
	TopFrontLeft->BBox = new AABB(Vector3(c.x - h, c.y + h, c.z + h), h);
	TopFrontLeft->parent = this;

	// Octant 1.
	TopFrontRight = new Octree();
	TopFrontRight->BBox = new AABB(Vector3(c.x + h, c.y + h, c.z + h), h);
	TopFrontRight->parent = this;

	// Octant 2.
	BottomFrontLeft = new Octree();
	BottomFrontLeft->BBox = new AABB(Vector3(c.x - h, c.y - h, c.z + h), h);
	BottomFrontLeft->parent = this;

	// Octant 3.
	BottomFrontRight = new Octree();
	BottomFrontRight->BBox = new AABB(Vector3(c.x + h, c.y - h, c.z + h), h);
	BottomFrontRight->parent = this;

	// Octant 4.
	TopBackLeft = new Octree();
	TopBackLeft->BBox = new AABB(Vector3(c.x - h, c.y + h, c.z - h), h);
	TopBackLeft->parent = this;

	// Octant 5.
	TopBackRight = new Octree();
	TopBackRight->BBox = new AABB(Vector3(c.x + h, c.y + h, c.z - h), h);
	TopBackRight->parent = this;

	// Octant 6.
	BottomBackLeft = new Octree();
	BottomBackLeft->BBox = new AABB(Vector3(c.x - h, c.y - h, c.z - h), h);
	BottomBackLeft->parent = this;

	// Octant 7.
	BottomBackRight = new Octree();
	BottomBackRight->BBox = new AABB(Vector3(c.x + h, c.y - h, c.z - h), h);
	BottomBackRight->parent = this;
}

///// <summary>
///// Find the nearest objects in the specified area.
///// </summary>
///// <param name="area">
///// A bounding box that encapsulates the area.
///// </param>
///// <returns>
///// A list of objects in the area. (For now just test points)
///// </returns>
//std::vector<HE_Face*>* Octree::FindNearest(AABB* area)
//{
//	if (BBox == nullptr) throw "Bounding box must not be null";
//
//	//TODO: figure out a way to cull things not in frustum, and cull back-faces. Automatically here.
//	//TODO: Quick check could be to cull aabb if clipped by frustum 
//	//TODO: Occlusion testing here
//
//	std::vector<HE_Face*>* nearestList = new std::vector<HE_Face*>();
//	Octree* node;
//	std::stack<Octree*> stack;
//	stack.push(this);
//
//	while (stack.size() > 0)
//	{
//		node = stack.top();
//		stack.pop();
//
//		// Automatically abort if the range does not intersect this octree
//		if (node->BBox->Intersects(area))
//		{
//			// Check objects at this octree level
//			for (int p = 0; p < node->Polygons->size(); p++)
//			{
//				if (area->Intersects(node->Polygons->at(p)))
//				{
//					HE_Face* nearestFace = node->Polygons->at(p);
//
//					nearestList->push_back(nearestFace);
//				}
//			}
//
//			// Terminate here, if there are no children
//			if (!node->IsEmpty())
//			{
//				// Continue the search frontier.
//				if (node->TopBackLeft != nullptr)
//				{
//					stack.push(node->TopBackLeft);
//				}
//				if (node->TopBackRight != nullptr)
//				{
//					stack.push(node->TopBackRight);
//				}
//				if (node->TopFrontLeft != nullptr)
//				{
//					stack.push(node->TopFrontLeft);
//				}
//				if (node->TopFrontRight != nullptr)
//				{
//					stack.push(node->TopFrontRight);
//				}
//
//				if (node->BottomBackLeft != nullptr)
//				{
//					stack.push(node->BottomBackLeft);
//				}
//				if (node->BottomBackRight != nullptr)
//				{
//					stack.push(node->BottomBackRight);
//				}
//				if (node->BottomFrontLeft != nullptr)
//				{
//					stack.push(node->BottomFrontLeft);
//				}
//				if (node->BottomFrontRight != nullptr)
//				{
//					stack.push(node->BottomFrontRight);
//				}
//			}
//		}
//	}
//
//	return nearestList;
//}

void Octree::FindNearestObjects(AABB area, std::vector<IndexedTriangleSet*>& nearestList)
{
	if (BBox == nullptr) throw "Bounding box must not be null";

	//TODO: figure out a way to cull things not in frustum, and cull back-faces. Automatically here.
	//TODO: Quick check could be to cull aabb if clipped by frustum 
	//TODO: Occlusion testing here

	//std::vector<IndexedTriangleSet*> nearestList = std::vector<IndexedTriangleSet*>(); // Check for memory leaks
	Octree* node;
	std::stack<Octree*> stack;
	stack.push(this);

	while (stack.size() > 0)
	{
		node = stack.top();
		stack.pop();

		// Automatically abort if the range does not intersect this octree
		if (node->BBox->Intersects(&area))
		{
			// Check objects at this octree level
			for (int p = 0; p < node->sceneObjects->size(); p++)
			{
				if (area.Intersects(node->sceneObjects->at(p)->Bounds))
				{
					IndexedTriangleSet* nearestITS = node->sceneObjects->at(p);

					nearestList.push_back(nearestITS);
				}
			}

			// Terminate here, if there are no children
			if (!node->IsEmpty())
			{
				// Continue the search frontier.
				if (node->TopBackLeft != nullptr)
				{
					stack.push(node->TopBackLeft);
				}
				if (node->TopBackRight != nullptr)
				{
					stack.push(node->TopBackRight);
				}
				if (node->TopFrontLeft != nullptr)
				{
					stack.push(node->TopFrontLeft);
				}
				if (node->TopFrontRight != nullptr)
				{
					stack.push(node->TopFrontRight);
				}

				if (node->BottomBackLeft != nullptr)
				{
					stack.push(node->BottomBackLeft);
				}
				if (node->BottomBackRight != nullptr)
				{
					stack.push(node->BottomBackRight);
				}
				if (node->BottomFrontLeft != nullptr)
				{
					stack.push(node->BottomFrontLeft);
				}
				if (node->BottomFrontRight != nullptr)
				{
					stack.push(node->BottomFrontRight);
				}
			}
		}
	}

	//return nearestList;
}