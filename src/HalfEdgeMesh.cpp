#include "HalfEdgeMesh.h"
#include "MathHelpers.h"

HalfEdgeMesh::HalfEdgeMesh()
{
	_tmpAdjacencyList = new HE_AdjacencyList();
}
//
//Octree* HalfEdgeMesh::CompressMeshOCT(Transform* modelTransform)
//{
//	int i;
//	HE_Face* face;
//	std::stack<HE_Face*> stack;
//	stack.push(GetRoot());
//
//	Octree* octTree = new Octree();
//
//	octTree->transform = modelTransform;
//
//	// Compute the AABB for the current mesh so the quad tree can subdivide later on.
//	octTree->BBox = ComputeBounds(modelTransform);
//
//	Matrix4 world_model = modelTransform->worldMatrix * modelTransform->localMatrix;
//
//	for (i = 0; i < _tmpAdjacencyList->FaceList->size(); i++) // BUG: have an issue with GC which is why _tmpAdjacencyList is used.
//	{
//		face = &_tmpAdjacencyList->FaceList->at(i);
//		// Transform the face to world space 
//
//		face->Model = &world_model;
//		//face->Transform(world_model);
//
//		// Add the face to the Quad Tree.
//		octTree->Add(face);
//	}
//
//	return octTree;
//}
//
//
//QuadTree* HalfEdgeMesh::CompressMesh(Transform* modelTransform)
//{
//	int i;
//	HE_Face* face;
//	std::stack<HE_Face*> stack;
//	stack.push(GetRoot());
//
//	QuadTree* quadTree = new QuadTree();
//
//	quadTree->transform = modelTransform;
//
//	// Compute the AABB for the current mesh so the quad tree can subdivide later on.
//	quadTree->BBox = ComputeBounds(modelTransform);
//
//	Matrix4 world_model = modelTransform->worldMatrix * modelTransform->localMatrix;
//
//	for (i = 0; i < _tmpAdjacencyList->FaceList->size(); i++) // BUG: have an issue with GC which is why _tmpAdjacencyList is used.
//	{
//		face = &_tmpAdjacencyList->FaceList->at(i);
//		// Transform the face to world space 
//
//		face->Model = &world_model;
//		//face->Transform(world_model);
//
//		// Add the face to the Quad Tree.
//		quadTree->Add(face);
//	}
//
//	return quadTree;
//}
//
//AABB* HalfEdgeMesh::ComputeBounds(Transform* modelTransform)
//{
//	Vector3 Min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
//	Vector3 Max = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);
//
//	// BUG: have an issue with GC which is why _tmpAdjacencyList is used.
//	std::vector<HE_Face> lst = *_tmpAdjacencyList->FaceList;
//	for (HE_Face f : lst)
//	{
//		HE_Face face = f;
//
//		// Vertex 0
//		if (face.t->vertex00.x < Min.x) Min.x = face.t->vertex00.x;
//		if (face.t->vertex00.x > Max.x) Max.x = face.t->vertex00.x;
//
//		if (face.t->vertex00.y < Min.y) Min.y = face.t->vertex00.y;
//		if (face.t->vertex00.y > Max.y) Max.y = face.t->vertex00.y;
//
//		if (face.t->vertex00.z < Min.z) Min.z = face.t->vertex00.z;
//		if (face.t->vertex00.z > Max.z) Max.z = face.t->vertex00.z;
//
//		// Vertex 1
//		if (face.t->vertex01.x < Min.x) Min.x = face.t->vertex01.x;
//		if (face.t->vertex01.x > Max.x) Max.x = face.t->vertex01.x;
//
//		if (face.t->vertex01.y < Min.y) Min.y = face.t->vertex01.y;
//		if (face.t->vertex01.y > Max.y) Max.y = face.t->vertex01.y;
//
//		if (face.t->vertex01.z < Min.z) Min.z = face.t->vertex01.z;
//		if (face.t->vertex01.z > Max.z) Max.z = face.t->vertex01.z;
//
//		// Vertex 2
//		if (face.t->vertex10.x < Min.x) Min.x = face.t->vertex10.x;
//		if (face.t->vertex10.x > Max.x) Max.x = face.t->vertex10.x;
//
//		if (face.t->vertex10.y < Min.y) Min.y = face.t->vertex10.y;
//		if (face.t->vertex10.y > Max.y) Max.y = face.t->vertex10.y;
//
//		if (face.t->vertex10.z < Min.z) Min.z = face.t->vertex10.z;
//		if (face.t->vertex10.z > Max.z) Max.z = face.t->vertex10.z;
//	}
//
//	Matrix4 world_model = modelTransform->worldMatrix * modelTransform->localMatrix;
//
//	Vector3 center = (Max - Min) / 2.0f;
//	Vector4 centerW = Vector4(center, 1.0f);
//
//	center = Matrix4::TransformVector(world_model, centerW).xyz();
//
//	float halfSize = (center - (Max - Min)).Magnitude();
//
//	AABB* bbox = new AABB(center, halfSize);
//
//	return bbox;
//}

//void ComputeBounds()
//{
//    BBox.ComputeBounds(this);
//}

//void UpdateBounds(Matrix4 objectRotation)
//{
//    BBox.UpdateBounds(objectRotation);
//}

HE_Face* HalfEdgeMesh::GetRoot()
{
	return _root;
}

void HalfEdgeMesh::SetRoot(HE_Face* newRoot)
{
	_root = newRoot;
}

bool HalfEdgeMesh::Any()
{
	return _root != nullptr;
}

bool HalfEdgeMesh::IsEmpty()
{
	return _root == nullptr;
}

//static HalfEdgeMesh* HalfEdgeMesh::ProcessWavefrontIntoMesh(WOBJ_Mesh polygonalMesh)
//{
//	HalfEdgeMesh* mesh = new HalfEdgeMesh();

//	foreach(WOBJ_Group group in polygonalMesh.Groups)
//	{
//		int[] indicies = group.Indicies;
//		Vector3[] verticies = polygonalMesh.MeshBuffer.Verticies;

//		if (group.PolygonType.Value == PolygonType::Quads)
//		{
//			IndexedQuadSet* quadSet = new IndexedQuadSet();
//			quadSet->Verticies = verticies;
//			quadSet->Indicies = indicies;
//			//quadSet->FaceColours = polygonalMesh.MeshBuffer.FaceColours;
//			quadSet->Normals = polygonalMesh.MeshBuffer.Normals;
//			quadSet->FaceColours = Primitives.CreateColoursPerTriangleFace(indicies);
//			quadSet->TextCoords = polygonalMesh.MeshBuffer.TextCoords;
//			quadSet->TexCoordIndicies = polygonalMesh.MeshBuffer.TexCoordIndicies;

//			mesh.AddQuadSet(quadSet);
//		}
//		if (group.PolygonType.Value == PolygonType::Triangles)
//		{
//			IndexedTriangleSet* triangleSet = new IndexedTriangleSet();
//			triangleSet->Verticies = verticies;
//			triangleSet->Indicies = indicies;
//			//triangleSet->FaceColours = polygonalMesh.MeshBuffer.FaceColours;
//			triangleSet->TextCoords = polygonalMesh.MeshBuffer.TextCoords;
//			triangleSet->Normals = polygonalMesh.MeshBuffer.Normals;
//			triangleSet->FaceColours = Primitives::CreateColoursPerTriangleFace(indicies);
//			triangleSet->TextCoords = polygonalMesh.MeshBuffer.TextCoords;
//			triangleSet->TexCoordIndicies = polygonalMesh.MeshBuffer.TexCoordIndicies;

//			mesh->AddTriangleSet(triangleSet);
//		}


//	}

//	// Process geometry into mesh.
//	mesh->FindAdjacencies();

//	return mesh;
//}

HalfEdgeMesh* HalfEdgeMesh::CreateDummyMesh()
{
	float width = 1, height = 1, z = -1;
	HalfEdgeMesh* dummy = new HalfEdgeMesh();

	HE_Face face0;
	HE_Face face1;

	HE_Vertex vertex0 = HE_Vertex(0, Vector3(-width, -height, z));
	HE_Vertex vertex1 = HE_Vertex(1, Vector3(width, -height, z));
	HE_Vertex vertex2 = HE_Vertex(2, Vector3(width, height, z));
	HE_Vertex vertex3 = HE_Vertex(3, Vector3(-width, height, z));

	// CHECK: Test see if just a Quad renders

	// Face 0
	HE_Edge* edge_0_0 = new HE_Edge(vertex0, face0, 0);
	HE_Edge* edge_0_1 = new HE_Edge(vertex1, face0, 1);
	HE_Edge* edge_0_2 = new HE_Edge(vertex2, face0, 2);

	face0.Edge = edge_0_0; // Set initial edge for face 0.

	edge_0_0->Next = edge_0_1;
	edge_0_1->Next = edge_0_2;
	edge_0_2->Next = edge_0_0;

	edge_0_0->Previous = edge_0_2;
	edge_0_1->Previous = edge_0_0;
	edge_0_2->Previous = edge_0_1;

	// Face 1
	HE_Edge* edge_1_3 = new HE_Edge(vertex0, face1, 0);
	HE_Edge* edge_1_4 = new HE_Edge(vertex2, face1, 1);
	HE_Edge* edge_1_5 = new HE_Edge(vertex3, face1, 2);

	face1.Edge = edge_1_3; // Set initial edge for face 1.

	edge_1_3->Next = edge_1_4;
	edge_1_4->Next = edge_1_5;
	edge_1_5->Next = edge_1_3;

	edge_1_3->Previous = edge_1_5;
	edge_1_4->Previous = edge_1_3;
	edge_1_5->Previous = edge_1_4;

	// HAS SHARED EDGES should figure out that edge_0_2 and edge_1_5 are shared

	// Set adjacency between face 0 and face 1.  
	// Shared edges are: edge_0_2 of face 0 (Vertex = vertex1, Next = vertex0)
	//  and edge_1_5 of face 1 (Vertex = vertex1, Next = vertex0)

	// CONNECT TO should do this
	// Tell edge_0_2 about edge_1_5
	edge_0_2->Other = edge_1_5;

	// Tell edge_1_5 about edge_0_2
	edge_1_5->Other = edge_0_2;


	////HACK: Hack so that current Triangle renderer can be used
	//Triangle* t0 = new Triangle(edge_0_0->Vertex->Attributes->Vertex, edge_0_1->Vertex->Attributes->Vertex, edge_0_2->Vertex->Attributes->Vertex);
	//Triangle* t1 = new Triangle(edge_1_3->Vertex->Attributes->Vertex, edge_1_4->Vertex->Attributes->Vertex, edge_1_5->Vertex->Attributes->Vertex);

	//t0->normal00 = t0->SurfaceNormal();
	//t0->normal01 = t0->SurfaceNormal();
	//t0->normal10 = t0->SurfaceNormal();

	//t1->normal00 = t1->SurfaceNormal();
	//t1->normal01 = t1->SurfaceNormal();
	//t1->normal10 = t1->SurfaceNormal();

	//face0.t = t0;
	//face1.t = t1;
	face0.FaceIndex = 0;
	face1.FaceIndex = 1;


	// Set the entry point in the mesh to point to the first face in the geometry.
	dummy->SetRoot(&face0);

	return dummy;
}

void HalfEdgeMesh::FindAdjacencies()
{
	_tmpAdjacencyList->FindAdjacencies(); // TODO: BUG: REALLY REALLY SLOW PROCESSING STARTUP  but this only has to be done once.

										  // At this point all edges with neighbours should be connected and the graph is complete. 
										  // The mesh can be traversed by setting the root to point to ANY initial vertex, edge, or face.
										  // This edge points to the last face it processed.

	FaceCount = _tmpAdjacencyList->FaceCount;

	if (IsEmpty())
	{
		SetRoot(_tmpAdjacencyList->EntryPoint);
	}

	//testTraverse();
}

void HalfEdgeMesh::testTraverse()
{
	HE_Face* face;
	HE_Edge* edge;
	std::map<HE_Face*, int> visitedFaces;
	std::stack<HE_Face*> stack;
	std::map<HE_Face*, int>::iterator it;
	std::map<HE_Face*, int>::iterator it_ins;
	stack.push(GetRoot());

	while (stack.size() > 0)
	{
		face = stack.top();
		stack.pop();

		it_ins = visitedFaces.begin();
		visitedFaces.insert(it_ins, std::pair<HE_Face*, int>(face, 1));

		edge = face->Edge;
		do
		{
			if (edge->Face != nullptr)
			{

				it = visitedFaces.find(edge->Face);
				if (it == visitedFaces.end())
				{
					// Map does not contain the Face, therefore 
					stack.push(edge->Face);
				}
			}

			edge = edge->Next;
		} while (edge != face->Edge);
	}

}

/// <summary>
/// Inserts the specified face into the geometry of the mesh.
/// </summary>
/// <param name="newFace"></param>
bool HalfEdgeMesh::Push(HE_Face* newFace)
{
	throw "NotImplementedException";
}

/// <summary>
/// Removes the specified face from the mesh.
/// </summary>
bool HalfEdgeMesh::Remove(HE_Face* face)
{
	// Search the graph to find the verticies that share a common edge 
	// Remove the face from adjacent faces and connect 
	// (similar to delete in doubly linked list)
	throw "NotImplementedException";
}

/// <summary>
/// Removes a face from an Edge.
/// </summary>
bool HalfEdgeMesh::RemoveFrom(HE_Face* face, HE_Edge* other)
{
	throw "NotImplementedException";
}

/// <summary>
/// Determines if the mesh contains the specified face geometry.
/// </summary>
bool HalfEdgeMesh::HasFace(HE_Face* face)
{
	throw "NotImplementedException";
}

/// <summary>
/// Determines if the mesh contains the specified edge geometry.
/// </summary>
bool HalfEdgeMesh::HasEdge(HE_Edge* edge)
{
	throw "NotImplementedException";
}

/// <summary>
/// Determines if the mesh contains the specified vertex.
/// </summary>
bool HalfEdgeMesh::HasVertex(HE_Vertex* vertex)
{
	throw "NotImplementedException";
}

/// <summary>
/// Gets a vertex from the mesh matching a vertex by the specified position.
/// </summary>
/// <param name="position"></param>
/// <returns></returns>
HE_Vertex HalfEdgeMesh::GetVertex(Vector3 position)
{
	throw "NotImplementedException";
}

HE_Edge HalfEdgeMesh::GetFace(Vector3 v00, Vector3 v01, Vector3 v10)
{
	throw "NotImplementedException";
}

HE_Face HalfEdgeMesh::GetFace(int faceIndex)
{
	throw "NotImplementedException";
}

HE_Edge HalfEdgeMesh::GetEdge(Vector3 begin, Vector3 end)
{
	throw "NotImplementedException";
}

HE_Edge HalfEdgeMesh::GetEdge(int vertexIndex, int edgeIndex)
{
	throw "NotImplementedException";
}

HE_Edge* HalfEdgeMesh::GetEdgesNearVertex(HE_Vertex* vertex)
{
	throw "NotImplementedException";
}

HE_Face* HalfEdgeMesh::GetFacesNearVertex(HE_Vertex* vertex)
{
	throw "NotImplementedException";
}

HE_Face* HalfEdgeMesh::GetFacesNearEdge(HE_Edge* edge)
{
	throw "NotImplementedException";
}

HE_Face* HalfEdgeMesh::GetFacesAdjacentToFace(HE_Face* face)
{
	throw "NotImplementedException";
}
