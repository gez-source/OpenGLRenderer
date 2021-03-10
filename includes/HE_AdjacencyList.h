#ifndef HE_ADJACENCY_LIST_H
#define HE_ADJACENCY_LIST_H

#include <vector>
#include "HE_Vertex.h"
#include "HE_Face.h"
#include "IndexedQuadSet.h"
#include "IndexedTriangleSet.h"

class HE_Vertex;
class HE_Face;
class IndexedQuadSet;
class IndexedTriangleSet;

class HE_AdjacencyList
{
public:
	HE_Face* EntryPoint;
	int FaceCount;
	std::vector<HE_Vertex>* VerticiesList;
	std::vector<HE_Face>* FaceList;

	HE_AdjacencyList();

	void AddQuadSet(IndexedQuadSet* quadSet);

	void AddTriangleSet(IndexedTriangleSet* triangleSet);

	void FindAdjacencies();
};

#endif
