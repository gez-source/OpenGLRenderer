#ifndef INDEXEDQUADSET_H
#define INDEXEDQUADSET_H

#include "Nullable.h"
#include "IndexedSet.h"
#include "ColourRGBA.h"

class IndexedQuadSet : public IndexedSet
{
public:
	Nullable<ColourRGBA> shapeColour;
};

#endif
