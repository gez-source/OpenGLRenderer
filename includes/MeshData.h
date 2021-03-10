#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <vector>
#include <GL/glew.h>

/**
 * This object contains all the mesh data for an animated model that is to be loaded into the VAO.
 *
 * @author Karl
 *
 */
class MeshData 
{
private:
	int DIMENSIONS = 3;

	int verticiesCount;
	int texCoordCount;
	int normalsCount;
	int indiciesCount;
	int jointIdsCount;
	int vertexWeightsCount;
	int vertexTangentsCount;
	int vertexBiTangentsCount;

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> textureCoords;
	std::vector<GLfloat> normals;
	std::vector<int> indices;
	std::vector<int> jointIds;
	std::vector<GLfloat> vertexWeights;
	std::vector<GLfloat> vertexTangents;
	std::vector<GLfloat> vertexBiTangents;

public:
	MeshData();

	MeshData(std::vector<GLfloat> vertices, int verticiesCount, std::vector<GLfloat> textureCoords, int texCoordCount, 
		std::vector<GLfloat> normals, int normalsCount, std::vector<int> indices, int indiciesCount,
		std::vector<int> jointIds, int jointIdsCount, std::vector<GLfloat> vertexWeights, int vertexWeightsCount,
		std::vector<GLfloat> vertexTangents, int vertexTangentsCount, std::vector<GLfloat> vertexBiTangents, int vertexBiTangentsCount);

	std::vector<GLfloat> getVertexBiTangents();
	std::vector<GLfloat> getVertexTangents();
	std::vector<int> getJointIds();
	std::vector<GLfloat> getVertexWeights();
	std::vector<GLfloat> getVertices();
	std::vector<GLfloat> getTextureCoords();
	std::vector<GLfloat> getNormals();
	std::vector<int> getIndices();
	int getVertexCount();

	int getVerticiesCount();
	int getTexCoordCount();
	int getNormalsCount();
	int getIndiciesCount();
	int getJointIdsCount();
	int getVertexWeightsCount();
	int getVertexTangentsCount();
	int getVertexBiTangentsCount();
};


#endif