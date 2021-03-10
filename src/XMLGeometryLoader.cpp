#include "XMLGeometryLoader.h"
#include "AnimatedModelLoader.h"
#include "MathHelpers.h"

GeometryLoader::GeometryLoader(pugi::xpath_node geometryNode, std::vector<VertexSkinData> vertexWeights)
{
	this->vertexWeights = vertexWeights;
	this->meshData = geometryNode.node().child("geometry").child("mesh");
}

MeshData* GeometryLoader::extractModelData()
{
	readRawData();
	assembleVertices();
	removeUnusedVertices();
	initArrays();
	convertDataToArrays();
	convertIndicesListToArray();

	return new MeshData(verticesArray, verticiesCount, texturesArray, texCoordsCount,
		normalsArray, normalsCount, indicesArray, indicesCount,
		jointIdsArray, jointsCount, weightsArray, weightsCount, 
		vertexTangents, vertexTangentsCount, vertexBiTangents, vertexBiTangentsCount
	);
}

void GeometryLoader::readRawData()
{
	readPositions();
	readNormals();
	readTextureCoords();
}

void GeometryLoader::readPositions()
{
	std::string positionsId = std::string(meshData.node().child("vertices").child("input").attribute("source").value()).substr(1);
	pugi::xpath_node positionsData = GetChildWithAttribute(meshData, "source", "id", positionsId).node().child("float_array");
	int count = std::stoi(std::string(positionsData.node().attribute("count").value()));
	std::vector<std::string> posData = StringSplit(std::string(positionsData.node().text().as_string()), " ");

	for (int i = 0; i < count / 3; i++)
	{
		float x = std::stof(posData[i * 3]);
		float y = std::stof(posData[i * 3 + 1]);
		float z = std::stof(posData[i * 3 + 2]);
		Vector4 position = Vector4(x, y, z, 1);

		//because in Blender z is up, but in our game y is up.
		position = Matrix4::TransformVector(AnimatedModelLoader::CORRECTION, position);

		if(vertexWeights.size() == 0)
		{
			VertexSkinData empty;
			vertices.push_back(new Vertex(vertices.size(), Vector3(position.x, position.y, position.z), empty));
		}
		else
		{
			vertices.push_back(new Vertex(vertices.size(), Vector3(position.x, position.y, position.z), vertexWeights.at(vertices.size())));
		}
	}
}

void GeometryLoader::readNormals()
{
	std::string normalsId;
	pugi::xpath_node inputNormal;

	hasNormals = false;

	if(HasChild(meshData.node(), "polylist"))
	{
		inputNormal = GetChildWithAttribute(meshData.node().child("polylist"), "input", "semantic", "NORMAL");

		if (!inputNormal.node().empty())
		{
			normalsId = std::string(inputNormal.node().attribute("source").value()).substr(1);
		}
	}
	else if (HasChild(meshData.node(), "triangles"))
	{
		inputNormal = GetChildWithAttribute(meshData.node().child("triangles"), "input", "semantic", "NORMAL");

		if (!inputNormal.node().empty())
		{
			normalsId = std::string(inputNormal.node().attribute("source").value()).substr(1);
		}
	}
	
	if (!normalsId.empty())
	{
		pugi::xpath_node normalsData = GetChildWithAttribute(meshData, "source", "id", normalsId).node().child("float_array");
		int count = std::stoi(std::string(normalsData.node().attribute("count").value()));
		std::vector<std::string> normData = StringSplit(std::string(normalsData.node().text().as_string()), " ");

		hasNormals = count > 0;

		for (int i = 0; i < count / 3; i++)
		{
			float x = std::stof(normData[i * 3]);
			float y = std::stof(normData[i * 3 + 1]);
			float z = std::stof(normData[i * 3 + 2]);
			Vector4 norm = Vector4(x, y, z, 0.0f);

			//because in Blender z is up, but in our game y is up.
			norm = Matrix4::TransformVector(AnimatedModelLoader::CORRECTION, norm);

			normals.push_back(Vector3(norm.x, norm.y, norm.z));
		}
	}
}

void GeometryLoader::readTextureCoords() 
{
	std::string texCoordsId;
	pugi::xpath_node inputTexCoord;

	hasTexCoords = false;

	if (HasChild(meshData.node(), "polylist"))
	{
		inputTexCoord = GetChildWithAttribute(meshData.node().child("polylist"), "input", "semantic", "TEXCOORD");

		if (!inputTexCoord.node().empty())
		{
			texCoordsId = std::string(inputTexCoord.node().attribute("source").value()).substr(1);
		}
	}
	else if (HasChild(meshData.node(), "triangles"))
	{
		inputTexCoord = GetChildWithAttribute(meshData.node().child("triangles"), "input", "semantic", "TEXCOORD");

		if (!inputTexCoord.node().empty())
		{
			texCoordsId = std::string(inputTexCoord.node().attribute("source").value()).substr(1);
		}
	}

	if (!texCoordsId.empty())
	{
		pugi::xpath_node texCoordsData = GetChildWithAttribute(meshData, "source", "id", texCoordsId).node().child("float_array");
		int count = texCoordsData.node().attribute("count").as_int();
		std::vector<std::string> texData = StringSplit(std::string(texCoordsData.node().text().as_string()), " ");

		hasTexCoords = count > 0;

		for (int i = 0; i < count / 2; i++)
		{
			float s = std::stof(texData[i * 2]);
			float t = std::stof(texData[i * 2 + 1]);

			textures.push_back(Vector2(s, t));
		}
	}
}

void GeometryLoader::assembleVertices() 
{
	pugi::xpath_node poly;
	if (HasChild(meshData.node(), "polylist"))
	{
		poly = meshData.node().child("polylist");
	}
	else if (HasChild(meshData.node(), "triangles"))
	{
		poly = meshData.node().child("triangles");
	}
	
	auto inputsChildren = poly.node().children("input");
	int typeCount = std::distance(inputsChildren.begin(), inputsChildren.end());
	std::vector<std::string> indexData = StringSplit(std::string(poly.node().child("p").text().as_string()), " ");

	for (int i = 0; i < indexData.size() / typeCount; i++)
	{
		int positionIndex = std::stoi(indexData[i * typeCount]);
		int normalIndex = -1; 
		int texCoordIndex = -1; 

		if (hasNormals) 
		{
			normalIndex = std::stoi(indexData[i * typeCount + 1]);
		}

		if (hasTexCoords)
		{
			texCoordIndex = std::stoi(indexData[i * typeCount + 2]);
		}

		processVertex(positionIndex, normalIndex, texCoordIndex);
	}
}


Vertex* GeometryLoader::processVertex(int posIndex, int normIndex, int texIndex)
{
	Vertex* currentVertex = vertices.at(posIndex);
	if (!currentVertex->isSet())
	{
		currentVertex->setTextureIndex(texIndex);
		currentVertex->setNormalIndex(normIndex);
		indices.push_back(posIndex);

		return currentVertex;
	}
	else 
	{
		return dealWithAlreadyProcessedVertex(currentVertex, texIndex, normIndex);
	}
}

std::vector<int> GeometryLoader::convertIndicesListToArray()
{
	this->indicesCount = indices.size();
	this->indicesArray = indices;
	return indicesArray;
}

float GeometryLoader::convertDataToArrays()
{
	Vector3 tangent;
	Vector3 bitangent;
	float furthestPoint = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex* currentVertex = vertices.at(i);
		if (currentVertex->getLength() > furthestPoint) 
		{
			furthestPoint = currentVertex->getLength();
		}
		Vector3 position = currentVertex->getPosition();
		verticesArray.push_back(position.x);
		verticesArray.push_back(position.y);
		verticesArray.push_back(position.z);

		if (hasTexCoords)
		{
			Vector2 textureCoord = textures.at(currentVertex->getTextureIndex());

			texturesArray.push_back(textureCoord.x);
			texturesArray.push_back(textureCoord.y);
		}
		else 
		{
			texturesArray.push_back(0);
			texturesArray.push_back(0);
		}

		//// Retrive normals from baked texture instead
		//if (normalTexture != nullptr && hasTexCoords)
		//{
		//	Vector2 textureCoord = textures.at(currentVertex->getTextureIndex());

		//	Vector3 normalVector = normalTexture->textureData->normal(textureCoord);

		//	normalsArray.push_back(normalVector.x);
		//	normalsArray.push_back(normalVector.y);
		//	normalsArray.push_back(normalVector.z);
		//}
		//else 
		//{
		//	if (hasNormals)
		//	{
		//		Vector3 normalVector = normals.at(currentVertex->getNormalIndex());

		//		normalsArray.push_back(normalVector.x);
		//		normalsArray.push_back(normalVector.y);
		//		normalsArray.push_back(normalVector.z);
		//	}
		//}

		if (hasNormals)
		{
			Vector3 normalVector = normals.at(currentVertex->getNormalIndex());

			normalsArray.push_back(normalVector.x);
			normalsArray.push_back(normalVector.y);
			normalsArray.push_back(normalVector.z);
		}
		
		VertexSkinData weights = currentVertex->getWeightsData();

		if(weights.jointIds.size() > 0)
		{
			jointIdsArray.push_back(weights.jointIds.at(0));
			jointIdsArray.push_back(weights.jointIds.at(1));
			jointIdsArray.push_back(weights.jointIds.at(2));
		}
		if (weights.weights.size() > 0)
		{
			weightsArray.push_back(weights.weights.at(0));
			weightsArray.push_back(weights.weights.at(1));
			weightsArray.push_back(weights.weights.at(2));
		}
	}

	vertexTangents = std::vector<GLfloat>(vertices.size() * 3);
	vertexBiTangents = std::vector<GLfloat>(vertices.size() * 3);

	if (hasTexCoords)
	{
		int i;
		int j;
		for (i = 0; i < indices.size(); i++)
		{
			int faceIndex = i % 3;

			if (faceIndex == 0)
			{
				int ind0 = indices.at(i);
				int ind1 = indices.at(i + 1);
				int ind2 = indices.at(i + 2);

				Vertex* cv0 = vertices.at(ind0);
				Vertex* cv1 = vertices.at(ind1);
				Vertex* cv2 = vertices.at(ind2);

				Vector3 v0 = cv0->getPosition();
				Vector3 v1 = cv1->getPosition();
				Vector3 v2 = cv2->getPosition();

				Vector2 t0 = textures.at(cv0->getTextureIndex());
				Vector2 t1 = textures.at(cv1->getTextureIndex());
				Vector2 t2 = textures.at(cv2->getTextureIndex());

				// Compute the Tangents and Bitangents
				TriangleTangentSpace(v0, v1, v2, t0, t1, t2, tangent, bitangent);


				vertexTangents[ind0 * 3] = tangent.x;
				vertexTangents[ind0 * 3 + 1] = tangent.y;
				vertexTangents[ind0 * 3 + 2] = tangent.z;

				vertexTangents[ind1 * 3] = tangent.x;
				vertexTangents[ind1 * 3 + 1] = tangent.y;
				vertexTangents[ind1 * 3 + 2] = tangent.z;

				vertexTangents[ind2 * 3] = tangent.x;
				vertexTangents[ind2 * 3 + 1] = tangent.y;
				vertexTangents[ind2 * 3 + 2] = tangent.z;


				vertexBiTangents[ind0 * 3] = bitangent.x;
				vertexBiTangents[ind0 * 3 + 1] = bitangent.y;
				vertexBiTangents[ind0 * 3 + 2] = bitangent.z;

				vertexBiTangents[ind1 * 3] = bitangent.x;
				vertexBiTangents[ind1 * 3 + 1] = bitangent.y;
				vertexBiTangents[ind1 * 3 + 2] = bitangent.z;

				vertexBiTangents[ind2 * 3] = bitangent.x;
				vertexBiTangents[ind2 * 3 + 1] = bitangent.y;
				vertexBiTangents[ind2 * 3 + 2] = bitangent.z;
			}
		}
	}

	return furthestPoint;
}

Vertex* GeometryLoader::dealWithAlreadyProcessedVertex(Vertex * previousVertex, int newTextureIndex, int newNormalIndex)
{
	if (previousVertex->hasSameTextureAndNormal(newTextureIndex, newNormalIndex)) {
		indices.push_back(previousVertex->getIndex());
		return previousVertex;
	}
	else {
		Vertex* anotherVertex = previousVertex->getDuplicateVertex();
		if (anotherVertex != nullptr) {
			return dealWithAlreadyProcessedVertex(anotherVertex, newTextureIndex, newNormalIndex);
		}
		else {
			Vertex* duplicateVertex = new Vertex(vertices.size(), previousVertex->getPosition(), previousVertex->getWeightsData());
			duplicateVertex->setTextureIndex(newTextureIndex);
			duplicateVertex->setNormalIndex(newNormalIndex);
			previousVertex->setDuplicateVertex(duplicateVertex);
			vertices.push_back(duplicateVertex);
			indices.push_back(duplicateVertex->getIndex());
			return duplicateVertex;
		}

	}
}

void GeometryLoader::initArrays()
{
	this->verticiesCount = vertices.size() * 3;
	this->texCoordsCount = vertices.size() * 2;
	this->normalsCount = vertices.size() * 3;
	this->jointsCount = vertices.size() * 3;
	this->weightsCount = vertices.size() * 3;
	this->vertexTangentsCount = vertices.size() * 3;
	this->vertexBiTangentsCount = vertices.size() * 3;
}

void GeometryLoader::removeUnusedVertices()
{
	for (Vertex* vertex : vertices)
	{
		vertex->averageTangents();

		if (!vertex->isSet())
		{
			vertex->setTextureIndex(0);
			vertex->setNormalIndex(0);
		}
	}
}