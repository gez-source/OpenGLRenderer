#ifndef HE_FACE_H
#define HE_FACE_H


#include "Matrix4.h"
#include "HE_Edge.h"
#include "HE_Pair.h"
#include "HE_Vertex.h"
#include "FaceAttributes.h"
#include <vector>

class HE_Pair;
class HE_Face;
class HE_Vertex;

class HE_Face
{
public:
	FaceAttributes* Attributes;

	//Triangle* t;
	int VertexCount = 0;
	int FaceIndex = -1;

	Matrix4* Model;

	/// One of the half-edges bordering the face.
	HE_Edge* Edge;

	HE_Face();

	~HE_Face();

	/// <summary>
	/// Applies a transformation matrix to all verticies in the face.
	/// </summary>
	/// <param name="tranform"></param>
	void Transform(Matrix4 tranform);

	Vector3 CenterOfMass();

	void ConnectTo(HE_Face* other);

	void ConnectAdjacent(HE_Face* other, HE_Pair* pair);

	bool HasSharedEdge(HE_Face* compareWith);

	bool HasSharedEdge(HE_Face* compareWith, HE_Pair* pair);

	// Static Methods

	/// <summary>
	/// Connects the left face (the new face) to the right face (existing face in geometry). 
	/// </summary>
	static void ConnectTo(HE_Face* left, HE_Face* right);

	/// <summary>
	/// Connects the left face to the right face by the specfied connectionPoint. (shared edge)
	/// </summary>
	static void ConnectAdjacent(HE_Face* left, HE_Face* right, HE_Pair* pair);

	/// <summary>
	/// Compares the specified face with the specified compareTo face 
	/// to see if they have a shared edge.
	/// </summary>
	/// <returns>
	/// Returns true if the faces border an edge, otherwise false.
	/// </returns>
	static bool HasSharedEdge(HE_Face* face, HE_Face* compareTo);

	/// <summary>
	/// Compares the specified face with the specified compareTo face 
	/// to see if they have a shared edge.
	/// </summary>
	/// <param name="connectionPoints">
	/// Outputs the shared edge.
	/// </param>
	/// <returns>
	/// Returns true if the faces border an edge, otherwise false.
	/// </returns>
	static bool HasSharedEdge(HE_Face* face, HE_Face* compareTo, HE_Pair* pair);


	//Triangle GetPrimitive();

	std::vector<HE_Vertex>* GetVertexes();

	std::vector<Vector3>* GetVerticies();

	std::vector<Vector3>* GetNormals();

	//static HE_Face* GetFace(IndexedTriangleSet* triangleSet, int faceIndex);

	//TODO: Refactor

	//void GetFace(IndexedTriangleSet* triangleSet, int faceIndex, int vertexSet);

	//TODO: Refactor
	//void GetEdges(int polyIndex, Vector3 v00, Vector3 v01, Vector3 v10, Triangle* t);

	/// <summary>
	/// Get the faces which border the half-edge.
	/// </summary>
	void GetAdjacentFaces(HE_Face* face1, HE_Face* face2);

	std::vector<HE_Face>* GetAdjacentFaces();

	std::vector<HE_Face>* GetNearestFaces();

	/// <summary>
	/// Get all the edges around the current Face.
	/// </summary>
	/// <returns></returns>
	std::vector<HE_Edge>* GetEdges();

	//std::vector<Triangle>* GetTriangles();
};

#endif
