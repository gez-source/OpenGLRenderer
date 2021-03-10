#include "IndexedTriangleSet.h"

IndexedTriangleSet* IndexedTriangleSet::ProcessIntoTriangleSet(IndexedQuadSet* quadSet)
{
	int vertexSet = 4;
	HE_Face he_face;
	HE_Face* he_face_prev = nullptr;
	HE_Face* he_face_next = nullptr;
	Quad* q;
	std::vector<Triangle>* primitivesList = new std::vector<Triangle>();
	IndexedTriangleSet* triangleSet;

	int indiciesCount = quadSet->Indicies->size();
	int faceCount = (indiciesCount / vertexSet);

	int faceIndex;
	for (faceIndex = 0; faceIndex < faceCount; faceIndex++)
	{
		int polyIndex = faceIndex * vertexSet;

		if((polyIndex + 3 > quadSet->Indicies->size()-1)
		|| (polyIndex + 2 > quadSet->Indicies->size()-1)
		|| (polyIndex + 1 > quadSet->Indicies->size()-1)
		|| (polyIndex > quadSet->Indicies->size()-1))
		{
            continue;
		}

		Vector3 vertex00 = quadSet->Verticies->at(quadSet->Indicies->at(polyIndex + 0));
		Vector3 vertex01 = quadSet->Verticies->at(quadSet->Indicies->at(polyIndex + 1));
		Vector3 vertex10 = quadSet->Verticies->at(quadSet->Indicies->at(polyIndex + 2));
		Vector3 vertex11 = quadSet->Verticies->at(quadSet->Indicies->at(polyIndex + 3));

		q = new Quad(vertex00, vertex01, vertex10, vertex11);

		if (quadSet->FaceColours != nullptr && faceIndex < ((quadSet->Indicies->size()) / vertexSet) - 1)
		{
			//q->faceColour = quadSet->FaceColours->at(faceIndex);
		}
		else
		{
			if (quadSet->shapeColour.is_set())
			{
				q->faceColour = quadSet->shapeColour.get();
			}
		}

		if (quadSet->Normals != nullptr && quadSet->Normals->size() > 0)
		{
			q->HasNormalPerVertex = true;

			if (quadSet->NormalIndicies != nullptr)
			{
				q->normal00 = quadSet->Normals->at(quadSet->NormalIndicies->at(polyIndex + 0));
				q->normal01 = quadSet->Normals->at(quadSet->NormalIndicies->at(polyIndex + 1));
				q->normal10 = quadSet->Normals->at(quadSet->NormalIndicies->at(polyIndex + 2));
				q->normal11 = quadSet->Normals->at(quadSet->NormalIndicies->at(polyIndex + 3));
			}
			else
			{
				q->normal00 = quadSet->Normals->at(quadSet->Indicies->at(polyIndex + 0));
				q->normal01 = quadSet->Normals->at(quadSet->Indicies->at(polyIndex + 1));
				q->normal10 = quadSet->Normals->at(quadSet->Indicies->at(polyIndex + 2));
				q->normal11 = quadSet->Normals->at(quadSet->Indicies->at(polyIndex + 3));
			}

		}
		else
		{
			q->AutocalcNormals = true;
		}

		if (quadSet->VertexColours != nullptr && quadSet->VertexColours->size() > 0)
		{
			q->HasColourPerVertex = true;

			q->colour00 = (quadSet->VertexColours->at(quadSet->Indicies->at(polyIndex + 0))).ToVec4();
			q->colour01 = (quadSet->VertexColours->at(quadSet->Indicies->at(polyIndex + 1))).ToVec4();
			q->colour10 = (quadSet->VertexColours->at(quadSet->Indicies->at(polyIndex + 2))).ToVec4();
			q->colour11 = (quadSet->VertexColours->at(quadSet->Indicies->at(polyIndex + 3))).ToVec4();
		}

		if (quadSet->TextCoords != nullptr && quadSet->TextCoords->size() > 0)
		{
			q->HasTexCoordPerVertex = true;

			if (quadSet->TexCoordIndicies != nullptr)
			{
				q->texCoord00 = quadSet->TextCoords->at(quadSet->TexCoordIndicies->at(polyIndex + 0));
				q->texCoord01 = quadSet->TextCoords->at(quadSet->TexCoordIndicies->at(polyIndex + 1));
				q->texCoord10 = quadSet->TextCoords->at(quadSet->TexCoordIndicies->at(polyIndex + 2));
				q->texCoord11 = quadSet->TextCoords->at(quadSet->TexCoordIndicies->at(polyIndex + 3));
			}
			else
			{
				q->texCoord00 = quadSet->TextCoords->at(quadSet->Indicies->at(polyIndex + 0));
				q->texCoord01 = quadSet->TextCoords->at(quadSet->Indicies->at(polyIndex + 1));
				q->texCoord10 = quadSet->TextCoords->at(quadSet->Indicies->at(polyIndex + 2));
				q->texCoord11 = quadSet->TextCoords->at(quadSet->Indicies->at(polyIndex + 3));
			}
		}

		// Process the Quad into two Triangles and add to list.
		std::vector<Triangle>* primitives = PrimitiveProcessor::ProcessQuad(q);

		primitivesList->insert(primitivesList->end(), primitives->begin(), primitives->end());
	}

	// TODO: IndexedTriangleSet.CreateIndexOf() doesnt re-index colours and texture coordiantes!
	triangleSet = IndexedTriangleSet::CreateIndexOf(primitivesList);

	return triangleSet;
}

IndexedTriangleSet* IndexedTriangleSet::CreateIndexOf(std::vector<Triangle>* triangles)
{
	IndexedTriangleSet* triangleSet = new IndexedTriangleSet();

	std::vector<Vector3>* verticiesList = new std::vector<Vector3>();
	std::vector<Vector3>* normalsList = new std::vector<Vector3>();
	std::vector<Vector2>* texCoordsList = new std::vector<Vector2>();

	// Loop through each triangle and build a collection of vertex buffers.

	for (int triIndex = 0; triIndex < triangles->size(); triIndex++)
	{
		Triangle t = triangles->at(triIndex);

		verticiesList->push_back(t.vertex00);
		verticiesList->push_back(t.vertex01);
		verticiesList->push_back(t.vertex10);

		if (t.HasVertexNormals())
		{
			normalsList->push_back(t.normal00.get());
			normalsList->push_back(t.normal01.get());
			normalsList->push_back(t.normal10.get());
		}

		if (t.HasVertexTexCoords())
		{
			texCoordsList->push_back(t.texCoord00.get());
			texCoordsList->push_back(t.texCoord01.get());
			texCoordsList->push_back(t.texCoord10.get());
		}

		//TODO: might want to index face colours and vertex colours
	}

	std::vector<int>* coordIndicies;
	std::vector<int>* normalIndicies;
	std::vector<int>* texCoordIndicies;
	std::vector<Vector3>* verticies = verticiesList;
	std::vector<Vector3>* normals = normalsList;
	std::vector<Vector2>* texCoords = texCoordsList;
	std::vector<Vector3>* verticiesIndexed;
	std::vector<Vector3>* normalsIndexed;
	std::vector<Vector2>* texCoordsIndexed;

	PrimitiveProcessor::Indexer(verticies, &coordIndicies, &verticiesIndexed);

	PrimitiveProcessor::Indexer(normals, &normalIndicies, &normalsIndexed);

	PrimitiveProcessor::Indexer(texCoords, &texCoordIndicies, &texCoordsIndexed);

	triangleSet->Indicies = coordIndicies;
	triangleSet->Verticies = verticiesIndexed;
	triangleSet->NormalIndicies = normalIndicies;
	triangleSet->Normals = normalsIndexed;
	triangleSet->TextCoords = texCoordsIndexed;
	triangleSet->TexCoordIndicies = texCoordIndicies;

	return triangleSet;
}
