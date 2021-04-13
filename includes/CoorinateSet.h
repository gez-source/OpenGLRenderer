#ifndef COORDINATE_SET_H
#define COORDINATE_SET_H

//class AABB;

#include "Vector3.h"
#include "Vector2.h"
#include "ColourRGBA.h"
#include <vector>

class CoorinateSet
{
public:
	std::vector<Vector3>* Verticies;
	std::vector<Vector2>* TextCoords;
	std::vector<Vector3>* Normals;
	std::vector<ColourRGBA>* FaceColours;
	std::vector<ColourRGBA>* VertexColours;

	std::vector<int>* NormalIndicies;
	std::vector<int>* TexCoordIndicies;
};

#endif
