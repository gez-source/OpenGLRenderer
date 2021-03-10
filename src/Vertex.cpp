#include "Vertex.h"

VertexSkinData Vertex::getWeightsData()
{
	return weightsData;
}

void Vertex::addTangent(Vector3 tangent)
{
	tangents->push_back(tangent);
}

void Vertex::averageTangents()
{
	if (tangents->empty()) {
		return;
	}
	for (auto const& tangent : *tangents)
	{
		averagedTangent = Vector3::Add(averagedTangent, tangent);
	}
	averagedTangent = averagedTangent.Normalise();
}

Vector3 Vertex::getAverageTangent()
{
	return averagedTangent;
}

int Vertex::getIndex()
{
	return index;
}

float Vertex::getLength()
{
	return length;
}

bool Vertex::isSet()
{
	return textureIndex != NO_INDEX && normalIndex != NO_INDEX;
}

bool Vertex::hasSameTextureAndNormal(int textureIndexOther, int normalIndexOther)
{
	return textureIndexOther == textureIndex && normalIndexOther == normalIndex;
}

void Vertex::setTextureIndex(int textureIndex)
{
	this->textureIndex = textureIndex;
}

void Vertex::setNormalIndex(int normalIndex)
{
	this->normalIndex = normalIndex;
}

Vector3 Vertex::getPosition()
{
	return position;
}

int Vertex::getTextureIndex()
{
	return textureIndex;
}

int Vertex::getNormalIndex()
{
	return normalIndex;
}

Vertex* Vertex::getDuplicateVertex()
{
	return duplicateVertex;
}

void Vertex::setDuplicateVertex(Vertex* duplicateVertex)
{
	this->duplicateVertex = duplicateVertex;
}