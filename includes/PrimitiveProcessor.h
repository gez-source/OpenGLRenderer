#ifndef PRIMITIVE_PROCESSOR_H
#define PRIMITIVE_PROCESSOR_H

#include "Triangle.h"
#include "Quad.h"
#include "IndexedQuadSet.h"
#include "IndexedTriangleSet.h"
#include "HalfEdgeMesh.h"
#include "Vector3.h"
#include "Vector2.h"

class HalfEdgeMesh;
class IndexedQuadSet;
class IndexedTriangleSet;

class PrimitiveProcessor
{
public:

	static HalfEdgeMesh* ProcessIntoHalfEdges(IndexedQuadSet* quadSet);

	static HalfEdgeMesh* ProcessIntoHalfEdges(IndexedTriangleSet* triangleSet);

	/// <summary>
	/// Process a Quad into two Triangles.
	/// </summary>
	static std::vector<Triangle>* ProcessQuad(Quad* q);

	/// <summary>
	/// Process a Quad into two Triangles.
	/// </summary>
	static void ProcessQuad(Quad* q, Triangle* t0, Triangle* t1);

	/// <summary>
	/// Unpack verticies from indexed set.
	/// </summary>
	static void Unpacker(std::vector<Vector3>* verticies, std::vector<int>* indicies, std::vector<Vector3>** verticiesUnpacked);

	/// <summary>
	/// Pack verticies into an indexed set.
	/// </summary>
	static void Indexer(std::vector<Vector3>* verticies, std::vector<int>** indicies,
		std::vector<Vector3>** indexedVerticies);

	/// <summary>
	/// Pack vectors into an indexed set.
	/// </summary>
	static void Indexer(std::vector<Vector2>* vectors, std::vector<int>** indicies,
		std::vector<Vector2>** indexedVerticies);
};

#endif
