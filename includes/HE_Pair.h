#ifndef HE_PAIR_H
#define HE_PAIR_H

#include "HE_Edge.h"

class HE_Edge;

class HE_Pair
{
public:
	HE_Edge* EdgeInner;
	HE_Edge* EdgeOpposite;
};

#endif
