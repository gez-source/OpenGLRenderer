#include "HE_AdjacencyList.h"

HE_AdjacencyList::HE_AdjacencyList()
{
	VerticiesList = new std::vector<HE_Vertex>();
	FaceList = new std::vector<HE_Face>();
}

void HE_AdjacencyList::AddQuadSet(IndexedQuadSet* quadSet)
{
	AddTriangleSet(IndexedTriangleSet::ProcessIntoTriangleSet(quadSet));
}

void HE_AdjacencyList::AddTriangleSet(IndexedTriangleSet* triangleSet)
{
	int vertexIndex;
	Vector3 vertPosition;
	HE_Vertex* vert;
	int verticiesCount = triangleSet->Verticies->size();

	// ADJACENCY INFORMATION per vertex.

	// Pass 1: Just find the vertexes.
	for (vertexIndex = 0; vertexIndex < verticiesCount; vertexIndex++)
	{
		vertPosition = triangleSet->Verticies->at(vertexIndex);

		vert = new HE_Vertex(vertexIndex, vertPosition);

		VerticiesList->push_back(*vert);
	}

	// Pass 2: Find the vertex edges.
	int faceIndex;
	int polyIndex;
	int vertexSet = 3;
	int faceCount = (triangleSet->Indicies->size() / vertexSet);
	HE_Face* face = nullptr;
	EntryPoint = nullptr;

	for (faceIndex = 0; faceIndex < faceCount; faceIndex++)
	{
		polyIndex = faceIndex * vertexSet;

		HE_Vertex vertex00 = VerticiesList->at(triangleSet->Indicies->at(polyIndex + 0));
		HE_Vertex vertex01 = VerticiesList->at(triangleSet->Indicies->at(polyIndex + 1));
		HE_Vertex vertex10 = VerticiesList->at(triangleSet->Indicies->at(polyIndex + 2));

		face = new HE_Face();

		face->GetFace(triangleSet, FaceCount, vertexSet);

		face->FaceIndex = faceIndex;
		//face.GetEdges(polyIndex, ref vertex00, ref vertex01, ref vertex10);
		FaceList->push_back(*face);




		// Each vertex now knows about the current face (by querying its edge 
		// and each vertex also now knows about half-edges within the current face.

		// Further work must be done to link faces to each other in pass 3.

		if (EntryPoint == nullptr) EntryPoint = face;

		FaceCount++;
	}
}

void HE_AdjacencyList::FindAdjacencies()
{
	// Pass 3: For each face, find neighbouring surfaces and imbue the knowledge of adjacency 
	//         by connecting the opposite oriented edges.
	int faceIndex;
	int faceIndex2;
	HE_Face* faceOpposite;
	HE_Face* face;
	HE_Pair* pair = nullptr;

	for (faceIndex = 0; faceIndex < FaceCount; faceIndex++)
	{
		face = &FaceList->at(faceIndex);

		for (faceIndex2 = 0; faceIndex2 < FaceCount; faceIndex2++)
		{
			faceOpposite = &FaceList->at(faceIndex2);

			if (faceIndex != faceIndex2)
			{

				// The faces are not the same, great. Now check if either of these faces has one shared edge.
				if (HE_Face::HasSharedEdge(face, faceOpposite, pair))
				{
					//Console.WriteLine("Face {0} has shared edge with face {1}", face.FaceIndex, faceOpposite.FaceIndex);

					HE_Face::ConnectAdjacent(face, faceOpposite, pair);

					FaceList->at(faceIndex) = *face;
					FaceList->at(faceIndex2) = *faceOpposite;
					//FaceList[faceIndex] = face;
					//FaceList[faceIndex2] = faceOpposite;
				}
			}
		}
	}
}
