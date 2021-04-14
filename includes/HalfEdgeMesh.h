// Half-edge Data Structure
// © Gerallt 2017 - 2020.

#ifndef HALF_EDGE_MESH_H
#define HALF_EDGE_MESH_H

#include <map>
#include <stack>
#include "Vector3.h"
#include "HE_Face.h"
#include "HE_AdjacencyList.h"
#include "Octree.h"
#include "Quadtree.h"

class Octree;
class Transform;
class HE_AdjacencyList;
class HE_Vertex;
class HE_Face;
class HE_Edge;
class QuadTree;

class HalfEdgeMesh
{
public:
	HE_Face* _root;
	HE_AdjacencyList* _tmpAdjacencyList;

	int FaceCount = 0;

	//[Obsolete("use AABB instead")]
	//BoundingBox BBox = new BoundingBox();

	
	HalfEdgeMesh();

	//Octree* CompressMeshOCT(Transform* modelTransform);
	//QuadTree* CompressMesh(Transform* modelTransform);
	//AABB* ComputeBounds(Transform* modelTransform);

	//void ComputeBounds();

	//void UpdateBounds(Matrix4 objectRotation);

	HE_Face* GetRoot();

	void SetRoot(HE_Face* newRoot);

	bool Any();

	bool IsEmpty();

	//static HalfEdgeMesh* ProcessWavefrontIntoMesh(WOBJ_Mesh polygonalMesh);

	static HalfEdgeMesh* CreateDummyMesh();

	void FindAdjacencies();

	void testTraverse();

	/// <summary>
	/// Inserts the specified face into the geometry of the mesh.
	/// </summary>
	/// <param name="newFace"></param>
	bool Push(HE_Face* newFace);

	/// <summary>
	/// Removes the specified face from the mesh.
	/// </summary>
	bool Remove(HE_Face* face);

	/// <summary>
	/// Removes a face from an Edge.
	/// </summary>
	bool RemoveFrom(HE_Face* face, HE_Edge* other);

	/// <summary>
	/// Determines if the mesh contains the specified face geometry.
	/// </summary>
	bool HasFace(HE_Face* face);

	/// <summary>
	/// Determines if the mesh contains the specified edge geometry.
	/// </summary>
	bool HasEdge(HE_Edge* edge);

	/// <summary>
	/// Determines if the mesh contains the specified vertex.
	/// </summary>
	bool HasVertex(HE_Vertex* vertex);

	/// <summary>
	/// Gets a vertex from the mesh matching a vertex by the specified position.
	/// </summary>
	/// <param name="position"></param>
	/// <returns></returns>
	HE_Vertex GetVertex(Vector3 position);

	HE_Edge GetFace(Vector3 v00, Vector3 v01, Vector3 v10);

	HE_Face GetFace(int faceIndex);

	HE_Edge GetEdge(Vector3 begin, Vector3 end);

	HE_Edge GetEdge(int vertexIndex, int edgeIndex);

	HE_Edge* GetEdgesNearVertex(HE_Vertex* vertex);

	HE_Face* GetFacesNearVertex(HE_Vertex* vertex);

	HE_Face* GetFacesNearEdge(HE_Edge* edge);

	HE_Face* GetFacesAdjacentToFace(HE_Face* face);
};

#endif
