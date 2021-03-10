#ifndef WAVEFRONT_MESH_IMPORTER_H
#define WAVEFRONT_MESH_IMPORTER_H

//#include <map>
#include "ColourRGBA.h"
#include "WOBJ_Mesh.h"
#include "ParserState.h"
#include "FileHelpers.h"
#include <fstream>
#include <iostream>
#include "WavefrontEnums.h"

const std::string TOKEN_DELIMITER = " ";
const std::string TOKEN_INDICIES_DELIMITER = "/";

class WavefrontOBJMeshImporter //TODO: Support mtl files for materials 
{
public:
	int								BufferSize = 128;

	// TEST  per face colours.
	//std::map<int, ColourRGBA>*		mapFaceColours = new std::map<int, ColourRGBA>();
	int								faceIDx = 0;

	WOBJ_Mesh* ImportFile(std::string fileName);
	void ImportWOBJMesh(std::string fileName, WOBJ_Mesh** meshObj);
	void processLineToken(ParserState state, OBJToken token);
	static void handleNewFaceGroup(ParserState state);
	OBJToken GetToken(std::string line);
	int ParseSmoothingGroups(std::string line);
	std::string ParseMTLLibLocation(std::string line);
	std::string ParseMaterialRef(std::string line);
	std::string ParseName(std::string line);
	bool try_parse_vector3(std::string line, Vector3** outVector);
	bool try_parse_vector2(std::string line, Vector2** outVector);
	Vector3 ParseVertexCoord(std::string line);
	Vector2 ParseVertexTextureCoordinate(std::string line);
	Vector3 ParseVertexNormal(std::string line);
	WOBJ_Face* ParseFace(std::string line);
};

#endif
