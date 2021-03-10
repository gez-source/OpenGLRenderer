#ifndef WOBJ_GROUP
#define WOBJ_GROUP

#include "Nullable.h"
#include "Enums.h"
#include <string>
#include <vector>


class WOBJ_Group
{
public:
	std::string GroupName;
	int BeginIndex;
	int EndIndex;
	std::vector<int>* Indicies;
	PolygonType Polygon;

	WOBJ_Group()
	{
		BeginIndex = 0;
		EndIndex = 0;
		Polygon = PolygonType::PT_Triangles;
	}
};

#endif
