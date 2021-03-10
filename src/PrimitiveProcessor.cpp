#include "PrimitiveProcessor.h"
#include <map>

HalfEdgeMesh* PrimitiveProcessor::ProcessIntoHalfEdges(IndexedQuadSet* quadSet)
{
	HalfEdgeMesh* mesh;
	IndexedTriangleSet* triangleSet;
	triangleSet = IndexedTriangleSet::ProcessIntoTriangleSet(quadSet);
	mesh = ProcessIntoHalfEdges(triangleSet);
	return mesh;
}

HalfEdgeMesh* PrimitiveProcessor::ProcessIntoHalfEdges(IndexedTriangleSet* triangleSet)
{
	HalfEdgeMesh* mesh = new HalfEdgeMesh();
	mesh->AddTriangleSet(triangleSet);
	return mesh;
}

/// <summary>
/// Process a Quad into two Triangles.
/// </summary>
std::vector<Triangle>* PrimitiveProcessor::ProcessQuad(Quad* q)
{
	Triangle* t0 = new Triangle();
	Triangle* t1 = new Triangle();

	ProcessQuad(q, t0, t1);

	std::vector<Triangle>* arr = new std::vector<Triangle>();
	arr->push_back(*t0);
	arr->push_back(*t1);

	return arr;
}

/// <summary>
/// Process a Quad into two Triangles.
/// </summary>
void PrimitiveProcessor::ProcessQuad(Quad* q, Triangle* t0, Triangle* t1)
{
	t0 = new Triangle();
	t1 = new Triangle();

	t0->faceColour = q->faceColour;
	t1->faceColour = q->faceColour;
	t0->HasNormalPerVertex = q->HasNormalPerVertex;
	t1->HasNormalPerVertex = q->HasNormalPerVertex;
	t0->HasColourPerVertex = q->HasColourPerVertex;
	t1->HasTexCoordPerVertex = q->HasTexCoordPerVertex;
	t0->HasTexCoordPerVertex = q->HasTexCoordPerVertex;

	t0->normal00 = q->normal00;
	t0->normal01 = q->normal01;
	t0->normal10 = q->normal10;

	t1->normal00 = q->normal10;
	t1->normal01 = q->normal11;
	t1->normal10 = q->normal00;


	t0->colour00 = q->colour00;
	t0->colour01 = q->colour01;
	t0->colour10 = q->colour10;

	t1->colour00 = q->colour10;
	t1->colour01 = q->colour11;
	t1->colour10 = q->colour00;


	t0->texCoord00 = q->texCoord00;
	t0->texCoord01 = q->texCoord01;
	t0->texCoord10 = q->texCoord10;

	t1->texCoord00 = q->texCoord10;
	t1->texCoord01 = q->texCoord11;
	t1->texCoord10 = q->texCoord00;


	t0->vertex00 = q->vertex00;
	t0->vertex01 = q->vertex01;
	t0->vertex10 = q->vertex10;

	t1->vertex00 = q->vertex10;
	t1->vertex01 = q->vertex11;
	t1->vertex10 = q->vertex00;

	t0->SwapVectors();
	t1->SwapVectors();
}

/// <summary>
/// Unpack verticies from indexed set.
/// </summary>
void PrimitiveProcessor::Unpacker(std::vector<Vector3>* verticies, std::vector<int>* indicies, std::vector<Vector3>** verticiesUnpacked)
{
	int coordIndex;
	Vector3 coordVertex;
	std::vector<Vector3>* unpackedList = new std::vector<Vector3>();

	for (coordIndex = 0; coordIndex < verticies->size(); coordIndex++)
	{
		coordVertex = verticies->at(coordIndex);

		unpackedList->push_back(coordVertex);
	}

	verticiesUnpacked = &unpackedList;
}

/// <summary>
/// Pack verticies into an indexed set.
/// </summary>
void PrimitiveProcessor::Indexer(std::vector<Vector3>* verticies, std::vector<int>** indicies,
	std::vector<Vector3>** indexedVerticies)
{
	int coordIndex;
	Vector3 coordVertex;
	std::vector<int>* index = new std::vector<int>();
	std::vector<Vector3>* indexedVerts = new std::vector<Vector3>();
	std::map<int, Vector3>* mapIndexToCoord = new std::map<int, Vector3>();
	std::map<Vector3, int>* mapCoordToIndex = new std::map<Vector3, int>();
	std::map<Vector3, int>::iterator it_ins0;
	std::map<int, Vector3>::iterator it_ins1;

	//List<int> index = new List<int>();
	//List<Vector3> indexedVerts = new List<Vector3>();
	//Dictionary<int, Vector3> mapIndexToCoord = new Dictionary<int, Vector3>();
	//Dictionary<Vector3, int> mapCoordToIndex = new Dictionary<Vector3, int>();
	int key = -1;
	bool mapContainsVal;

	for (coordIndex = 0; coordIndex < verticies->size(); coordIndex++)
	{
		coordVertex = verticies->at(coordIndex);

		mapContainsVal = false;
		//foreach(Vector3 val in mapIndexToCoord.Values)
		for (std::map<int, Vector3>::iterator it = mapIndexToCoord->begin(); it != mapIndexToCoord->end(); ++it)
		{
			Vector3 val = it->second;
			if ((val.x == coordVertex.x) && (val.y == coordVertex.y) && (val.z == coordVertex.z))
			{
				mapContainsVal = true;
				break;
			}
		}
		
		if (!mapContainsVal)
		{
			// Map a unique verticy to an index.
			it_ins1 = mapIndexToCoord->begin();
			mapIndexToCoord->insert(it_ins1, std::pair<int, Vector3>(++key, coordVertex));

			// Map an index to the unique coordinate.
			it_ins0 = mapCoordToIndex->begin();
			mapCoordToIndex->insert(it_ins0, std::pair<Vector3, int>(coordVertex, key));

			// Add each unique verticy to a list.
			indexedVerts->push_back(coordVertex);
		}
	}

	for (coordIndex = 0; coordIndex < verticies->size(); coordIndex++)
	{
		coordVertex = verticies->at(coordIndex);

		index->push_back(mapCoordToIndex->at(coordVertex));
	}

	indexedVerticies = &indexedVerts;
	indicies = &index;
}

/// <summary>
/// Pack vectors into an indexed set.
/// </summary>
void PrimitiveProcessor::Indexer(std::vector<Vector2>* vectors, std::vector<int>** indicies,
	std::vector<Vector2>** indexedVerticies)
{
	int coordIndex;
	Vector2 coordVertex;
	std::vector<int>* index = new std::vector<int>();
	std::vector<Vector2>* indexedVerts = new std::vector<Vector2>();
	std::map<int, Vector2>* mapIndexToCoord = new std::map<int, Vector2>();
	std::map<Vector2, int>* mapCoordToIndex = new std::map<Vector2, int>();
	std::map<Vector2, int>::iterator it_ins0;
	std::map<int, Vector2>::iterator it_ins1;
	int key = -1;
	bool mapContainsVal;
	int vectorCount = vectors->size();

	for (coordIndex = 0; coordIndex < vectorCount; coordIndex++)
	{
		coordVertex = vectors->at(coordIndex);

		mapContainsVal = false;
		//foreach(Vector2 val in mapIndexToCoord.Values)
		for (std::map<int, Vector2>::iterator it = mapIndexToCoord->begin(); it != mapIndexToCoord->end(); ++it)
		{
			Vector3 val = it->second;
			if ((val.x == coordVertex.x) && (val.y == coordVertex.y))
			{
				mapContainsVal = true;
				break;
			}
		}

		if (!mapContainsVal)
		{
			// Map a unique verticy to an index.
			it_ins1 = mapIndexToCoord->begin();
			mapIndexToCoord->insert(it_ins1, std::pair<int, Vector2>(++key, coordVertex));

			// Map an index to the unique coordinate.
			it_ins0 = mapCoordToIndex->begin();
			mapCoordToIndex->insert(it_ins0, std::pair<Vector2, int>(coordVertex, key));

			// Add each unique verticy to a list.
			indexedVerts->push_back(coordVertex);
		}
	}

	for (coordIndex = 0; coordIndex < vectorCount; coordIndex++)
	{
		coordVertex = vectors->at(coordIndex);

		index->push_back(mapCoordToIndex->at(coordVertex));
	}

	indexedVerticies = &indexedVerts;
	indicies = &index;
}
