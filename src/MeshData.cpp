#include "MeshData.h"

MeshData::MeshData() : verticiesCount(0), texCoordCount(0), normalsCount(0), indiciesCount(0), jointIdsCount(0), vertexWeightsCount(0), vertexTangentsCount(0), vertexBiTangentsCount(0)
{

}

MeshData::MeshData(std::vector<GLfloat> vertices, int verticiesCount, std::vector<GLfloat> textureCoords, int texCoordCount,
	std::vector<GLfloat> normals, int normalsCount, std::vector<int> indices, int indiciesCount,
	std::vector<int> jointIds, int jointIdsCount, std::vector<GLfloat> vertexWeights, int vertexWeightsCount,
	std::vector<GLfloat> vertexTangents, int vertexTangentsCount, std::vector<GLfloat> vertexBiTangents, int vertexBiTangentsCount)
{
	this->vertices = vertices;
	this->textureCoords = textureCoords;
	this->normals = normals;
	this->indices = indices;
	this->jointIds = jointIds;
	this->vertexWeights = vertexWeights;
	this->vertexTangents = vertexTangents;
	this->vertexBiTangents = vertexBiTangents;

	this->verticiesCount = verticiesCount;
	this->texCoordCount = texCoordCount;
	this->normalsCount = normalsCount;
	this->indiciesCount = indiciesCount;
	this->jointIdsCount = jointIdsCount;
	this->vertexWeightsCount = vertexWeightsCount;
	this->vertexTangentsCount = vertexTangentsCount;
	this->vertexBiTangentsCount = vertexBiTangentsCount;
}

std::vector<GLfloat> MeshData::getVertexBiTangents()
{
	return vertexBiTangents; 
}

std::vector<GLfloat> MeshData::getVertexTangents()
{
	return vertexTangents;
}

std::vector<int> MeshData::getJointIds()
{
	return jointIds;
}

std::vector<GLfloat> MeshData::getVertexWeights()
{
	return vertexWeights;
}

std::vector<GLfloat> MeshData::getVertices()
{
	return vertices;
}

std::vector<GLfloat> MeshData::getTextureCoords()
{
	return textureCoords;
}

std::vector<GLfloat> MeshData::getNormals()
{
	return normals;
}

std::vector<int> MeshData::getIndices()
{
	return indices;
}

int MeshData::getVertexCount()
{
	return verticiesCount / DIMENSIONS;
}

int MeshData::getVerticiesCount() 
{
	return verticiesCount;
}

int MeshData::getTexCoordCount()
{
	return texCoordCount;
}

int MeshData::getNormalsCount() 
{
	return normalsCount;
}

int MeshData::getIndiciesCount()
{
	return indiciesCount;
}

int MeshData::getJointIdsCount() 
{
	return jointIdsCount;
}

int MeshData::getVertexWeightsCount()
{
	return vertexWeightsCount;
}

int MeshData::getVertexTangentsCount()
{
	return vertexTangentsCount;
}

int MeshData::getVertexBiTangentsCount()
{
	return vertexBiTangentsCount; 
}