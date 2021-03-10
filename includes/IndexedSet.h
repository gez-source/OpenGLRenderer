#ifndef INDEXEDSET_H
#define INDEXEDSET_H

//#include <vector>

//class CoorinateSet;

#include "CoorinateSet.h"

class IndexedSet : public CoorinateSet
{
public:
	std::vector<int>* Indicies;
	// TODO: Index Normals and other attributes as well!
};

#endif
