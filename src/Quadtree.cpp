//#include "Quadtree.h"
//
///// <summary>
///// Check if the current QuadTree is empty.
///// </summary>
///// <returns>
///// Returns true if the tree is empty.
///// </returns>
//bool QuadTree::IsEmpty()
//{
//	return TopLeft == nullptr && TopRight == nullptr && BottomLeft == nullptr && BottomRight == nullptr;
//}
//
//bool QuadTree::Add(QuadTree* quadTree)
//{
//	if (BBox == nullptr) throw "Bounding box must not be null. Just calculate the BBox for the root QuadTree and the decendants are calcualted automatically.";
//
//	bool inserted = false;
//	QuadTree* node;
//	std::stack<QuadTree*> stack;
//	stack.push(this);
//
//	while (stack.size() > 0 && !inserted)
//	{
//		node = stack.top();
//		stack.pop();
//
//		// Ignore areas that the object (p) does not belong in.
//		if (node->BBox->Intersects(quadTree->BBox))
//		{
//			if (node->IsEmpty())
//			{
//				node->Subdivide();
//
//				// Create a point in the tree where the existing quadtree belongs.
//
//				node->TopLeft = quadTree;
//				inserted = true;
//			}
//			else
//			{
//				// Continue the search frontier.
//				if (node->TopLeft != nullptr)
//				{
//					stack.push(node->TopLeft);
//				}
//				if (node->TopRight != nullptr)
//				{
//					stack.push(node->TopRight);
//				}
//				if (node->BottomLeft != nullptr)
//				{
//					stack.push(node->BottomLeft);
//				}
//				if (node->BottomRight != nullptr)
//				{
//					stack.push(node->BottomRight);
//				}
//			}
//		}
//	}
//
//	return inserted;
//}
//
///// <summary>
///// Insert a point into the tree.
///// </summary>
///// <returns>
///// Returns true if the point could be inserted in the tree, 
///// otherwise false if the object could not be added. 
///// </returns>
//bool QuadTree::Add(HE_Face* polygon)
//{
//	if (BBox == nullptr) throw "Bounding box must not be null. Just calculate the BBox for the root QuadTree and the decendants are calcualted automatically.";
//
//	bool inserted = false;
//	QuadTree* node;
//	std::stack<QuadTree*> stack;
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
//				if (node->TopLeft != nullptr)
//				{
//					stack.push(node->TopLeft);
//				}
//				if (node->TopRight != nullptr)
//				{
//					stack.push(node->TopRight);
//				}
//				if (node->BottomLeft != nullptr)
//				{
//					stack.push(node->BottomLeft);
//				}
//				if (node->BottomRight != nullptr)
//				{
//					stack.push(node->BottomRight);
//				}
//			}
//		}
//	}
//
//	return inserted;
//}
//
//void QuadTree::Subdivide()
//{
//	if (BBox == nullptr) throw "Bounding box must not be null";
//
//	// Given the current bounding box, divide the current half size and center by 2
//	// and then use these new values to create 4 new quad children of equal area.
//
//	float halfSize = BBox->HalfSize / 2.0f;
//
//	TopLeft = new QuadTree();
//	TopLeft->BBox = new AABB(Vector3(BBox->Center.x - halfSize, BBox->Center.y - halfSize, 0), halfSize);
//	TopLeft->parent = this;
//
//	TopRight = new QuadTree();
//	TopRight->BBox = new AABB(Vector3(BBox->Center.x + halfSize, BBox->Center.y - halfSize, 0), halfSize);
//	TopRight->parent = this;
//
//	BottomLeft = new QuadTree();
//	BottomLeft->BBox = new AABB(Vector3(BBox->Center.x - halfSize, BBox->Center.y + halfSize, 0), halfSize);
//	BottomLeft->parent = this;
//
//	BottomRight = new QuadTree();
//	BottomRight->BBox = new AABB(Vector3(BBox->Center.x + halfSize, BBox->Center.y + halfSize, 0), halfSize);
//	BottomRight->parent = this;
//}
//
///// <summary>
///// Find the nearest objects in the specified area.
///// </summary>
///// <param name="area">
///// A bounding box that encapsulates the area.
///// </param>
///// <returns>
///// A list of objects in the area. (For now just test points)
///// </returns>
//std::vector<HE_Face*>* QuadTree::FindNearest(AABB* area)
//{
//	if (BBox == nullptr) throw "Bounding box must not be null";
//
//	//TODO: figure out a way to cull things not in frustum, and cull back-faces. Automatically here.
//	//TODO: Quick check could be to cull aabb if clipped by frustum 
//	//TODO: Occlusion testing here
//
//	std::vector<HE_Face*>* nearestList = new std::vector<HE_Face*>();
//	QuadTree* node;
//	std::stack<QuadTree*> stack;
//	stack.push(this);
//
//	while (stack.size() > 0)
//	{
//		node = stack.top();
//		stack.pop();
//
//		// Automatically abort if the range does not intersect this quad
//		if (node->BBox->Intersects(area))
//		{
//			// Check objects at this quad level
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
//				if (node->TopLeft != nullptr)
//				{
//					stack.push(node->TopLeft);
//				}
//				if (node->TopRight != nullptr)
//				{
//					stack.push(node->TopRight);
//				}
//				if (node->BottomLeft != nullptr)
//				{
//					stack.push(node->BottomLeft);
//				}
//				if (node->BottomRight != nullptr)
//				{
//					stack.push(node->BottomRight);
//				}
//			}
//		}
//	}
//
//	return nearestList;
//}
