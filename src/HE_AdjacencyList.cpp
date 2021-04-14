#include "HE_AdjacencyList.h"

HE_AdjacencyList::HE_AdjacencyList()
{
	VerticiesList = new std::vector<HE_Vertex>();
	FaceList = new std::vector<HE_Face>();
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
