#ifndef INDEXEDTRIANGLESET_H
#define INDEXEDTRIANGLESET_H

class Quad;
class Triangle;
//class IndexedQuadSet;
//class IndexedSet;
//class ColourRGBA;

#include "Nullable.h"
#include "IndexedSet.h"
#include "HE_Face.h"
#include "IndexedQuadSet.h"
#include "Quad.h"
#include "PrimitiveProcessor.h"

class IndexedTriangleSet : public IndexedSet
{
public:

	Nullable<ColourRGBA> shapeColour;

	IndexedTriangleSet() : shapeColour(Nullable<ColourRGBA>())
	{
		
	}

	static IndexedTriangleSet* ProcessIntoTriangleSet(IndexedQuadSet* quadSet);

	static IndexedTriangleSet* CreateIndexOf(std::vector<Triangle>* triangles);
};

#endif
