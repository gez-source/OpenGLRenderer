#ifndef PARSER_STATE_H
#define PARSER_STATE_H

#include "WOBJ_Mesh.h"
#include "WavefrontEnums.h"
#include <string>
#include "WOBJ_Face.h"
#include "Nullable.h"
#include "AABB.h"
//#include <minwindef.h>

class ParserState
{
public:
	std::ifstream*	file;
	//FileStream filestream;
	//StreamReader file;
	WOBJ_Mesh* mesh;
	OBJToken token;
	std::string line;
	std::string objName;
	std::string groupName;
	Vector3 vertexCoord;
	Vector3 normalCoord;
	Vector2 texCoord;
	WOBJ_Face* face;
	std::string mtlFile;
	std::string materialRef;
	int numSmoothingGroups;
	int minFaceIndex = INT32_MAX;
	int maxFaceIndex = INT32_MIN;
	bool atFace = false;
	std::vector<Vector3>* verticiesList;
	std::vector<Vector3>* normalsList;
	std::vector<Vector2>* texCoordList;
	std::vector<int>* normalIndicies;
	std::vector<int>* texCoordIndicies;
	std::vector<int>* indicies;
	std::vector<ColourRGBA>* colours;
	PolygonType polyType;
	WOBJ_Group* group;

	ParserState()
	{
		group = new WOBJ_Group();
		colours = new std::vector<ColourRGBA>();

		verticiesList = new std::vector<Vector3>();
		normalsList = new std::vector<Vector3>();
		texCoordList = new std::vector<Vector2>();
		normalIndicies = new std::vector<int>();
		texCoordIndicies = new std::vector<int>();
		indicies = new std::vector<int>();
		indicies = new std::vector<int>();
		polyType = PolygonType::PT_Triangles;
	}
};

#endif
