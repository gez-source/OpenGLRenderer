#ifndef WOBJ_FACE_H
#define WOBJ_FACE_H
#include <vector>
#define __STDC_LIMIT_MACROS
#include <stdint.h>

class WOBJ_Face
{
public:
	int VertexCount;

	std::vector<int>* VertexCoordIndicies;
	std::vector<int>* VertexTexCoordIndicies;
	std::vector<int>* VertexNormalIndicies;

	bool HasNormals;
	bool HasTextureCoords;

	int MinVertIndex = INT32_MAX;
	int MaxVertIndex = INT32_MIN;

	WOBJ_Face()
	{
		VertexCount = 0;
		
		HasNormals = false;
		HasTextureCoords = false;

		VertexCoordIndicies = new std::vector<int>();
		VertexTexCoordIndicies = new std::vector<int>();
		VertexNormalIndicies = new std::vector<int>();
	}
};

#endif
