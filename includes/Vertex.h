#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "Vector3.h"
#include "VertexSkinData.h"

class Vertex 
{
private:
	int NO_INDEX = -1;

	Vector3 position;
	int textureIndex = NO_INDEX;
	int normalIndex = NO_INDEX;
	Vertex* duplicateVertex = nullptr;
	int index;
	float length;
	std::vector<Vector3>* tangents = new std::vector<Vector3>();
	Vector3 averagedTangent = Vector3(0, 0, 0);
	VertexSkinData weightsData;

public:

	Vertex() : index(0), length(0)
	{

	}

	Vertex(int index, Vector3 position, VertexSkinData weightsData) 
	{
		this->index = index;
		this->weightsData = weightsData;
		this->position = position;
		this->length = 3; // 3 component float vector
	}

	VertexSkinData getWeightsData();
	void addTangent(Vector3 tangent);
	void averageTangents();
	Vector3 getAverageTangent();
	int getIndex();
	float getLength();
	bool isSet();
	bool hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther);
	void setTextureIndex(int textureIndex);
	void setNormalIndex(int normalIndex);
	Vector3 getPosition();
	int getTextureIndex();
	int getNormalIndex();
	Vertex* getDuplicateVertex();
	void setDuplicateVertex(Vertex* duplicateVertex);

};

#endif