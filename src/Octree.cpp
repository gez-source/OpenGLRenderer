#include "Octree.h"

/// <summary>
/// Check if the current Octree is empty.
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

/// <summary>
/// Insert a mesh buffer VAO into the tree.
/// </summary>
/// <returns>
/// Returns true if the mesh buffer VAO could be inserted into the tree,
/// otherwise false if the object could not be added.
/// </returns>
bool Octree::Add(MeshBufferVAO* meshBufferVAO)
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
		if (meshBufferVAO->Intersects(node->BBox))
		{
			// Only add an object under this node if under capacity. This should create a balanced tree.
			if (node->sceneObjects->size() < node->CapacityThreshold)
			{
				node->sceneObjects->push_back(meshBufferVAO);
				inserted = true;
			}
			else
			{
				// Otherwise, subdivide and then add the point to whichever node will accept it.
				if (node->IsEmpty())
				{
					node->Subdivide();
				}

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

void Octree::Subdivide()
{
	if (BBox == nullptr) throw "Bounding box must not be null";

	// Given the current bounding box, divide the current half size and center by 2
	// and then use these new values to create 4 new quad children of equal area.

	// Divide the current half size by two to calculate the AABB's of the new children:
	Vector3 h = BBox->HalfSize / 2; // Easier to read.

	Vector3 c = BBox->Center;

	// Subdivide into 8 octants or 3 planes. (the octants are like 8 mini cubes stacked together)

	// Octant 0.
	TopFrontLeft = new Octree();
	TopFrontLeft->BBox = new AABB(Vector3(c.x - h.x, c.y + h.y, c.z + h.z), h);
	TopFrontLeft->parent = this;

	// Octant 1.
	TopFrontRight = new Octree();
	TopFrontRight->BBox = new AABB(Vector3(c.x + h.x, c.y + h.y, c.z + h.z), h);
	TopFrontRight->parent = this;

	// Octant 2.
	BottomFrontLeft = new Octree();
	BottomFrontLeft->BBox = new AABB(Vector3(c.x - h.x, c.y - h.y, c.z + h.z), h);
	BottomFrontLeft->parent = this;

	// Octant 3.
	BottomFrontRight = new Octree();
	BottomFrontRight->BBox = new AABB(Vector3(c.x + h.x, c.y - h.y, c.z + h.z), h);
	BottomFrontRight->parent = this;

	// Octant 4.
	TopBackLeft = new Octree();
	TopBackLeft->BBox = new AABB(Vector3(c.x - h.x, c.y + h.y, c.z - h.z), h);
	TopBackLeft->parent = this;

	// Octant 5.
	TopBackRight = new Octree();
	TopBackRight->BBox = new AABB(Vector3(c.x + h.x, c.y + h.y, c.z - h.z), h);
	TopBackRight->parent = this;

	// Octant 6.
	BottomBackLeft = new Octree();
	BottomBackLeft->BBox = new AABB(Vector3(c.x - h.x, c.y - h.y, c.z - h.z), h);
	BottomBackLeft->parent = this;

	// Octant 7.
	BottomBackRight = new Octree();
	BottomBackRight->BBox = new AABB(Vector3(c.x + h.x, c.y - h.y, c.z - h.z), h);
	BottomBackRight->parent = this;
}

void Octree::FindNearestObjects(AABB area, std::vector<MeshBufferVAO*>& nearestList)
{
	if (BBox == nullptr) throw "Bounding box must not be null";

	//TODO: figure out a way to cull things not in frustum, and cull back-faces. Automatically here.
	//TODO: Quick check could be to cull aabb if clipped by frustum 
	//TODO: Occlusion testing here

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
					MeshBufferVAO* nearest = node->sceneObjects->at(p);

					nearestList.push_back(nearest);
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
}