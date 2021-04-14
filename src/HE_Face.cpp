#include "HE_Face.h"


HE_Face::HE_Face()
{
	Attributes = new FaceAttributes();
}

HE_Face::~HE_Face()
{
	delete Attributes;
}

/// <summary>
/// Applies a transformation matrix to all verticies in the face.
/// </summary>
/// <param name="tranform"></param>
void HE_Face::Transform(Matrix4 tranform)
{
	//Vector4 v0 = Vector4(t->vertex00, 1.0f);
	//Vector4 v1 = Vector4(t->vertex01, 1.0f);
	//Vector4 v2 = Vector4(t->vertex10, 1.0f);

	//t->vertex00 = Matrix4::TransformVector(tranform, v0).xyz();
	//t->vertex01 = Matrix4::TransformVector(tranform, v1).xyz();
	//t->vertex10 = Matrix4::TransformVector(tranform, v2).xyz();

	//TODO: also update Vertex data in edge loop.
}

Vector3 HE_Face::CenterOfMass()
{
	//return t->CenterOfTriangle();
	return Vector3::Zero;
}

void HE_Face::ConnectTo(HE_Face* other)
{
	ConnectTo(this, other);
}

void HE_Face::ConnectAdjacent(HE_Face* other, HE_Pair* pair)
{
	ConnectAdjacent(this, other, pair);
}

bool HE_Face::HasSharedEdge(HE_Face* compareWith)
{
	return HasSharedEdge(this, compareWith);
}

bool HE_Face::HasSharedEdge(HE_Face* compareWith, HE_Pair* pair)
{
	return HasSharedEdge(this, compareWith, pair);
}

// Static Methods

/// <summary>
/// Connects the left face (the new face) to the right face (existing face in geometry). 
/// </summary>
void HE_Face::ConnectTo(HE_Face* left, HE_Face* right)
{
	HE_Pair edgePair;

	if (HasSharedEdge(left, right, &edgePair))
	{
		ConnectAdjacent(left, right, &edgePair);
	}
}

/// <summary>
/// Connects the left face to the right face by the specfied connectionPoint. (shared edge)
/// </summary>
void HE_Face::ConnectAdjacent(HE_Face* left, HE_Face* right, HE_Pair* pair)
{
	if (pair != nullptr)
	{
		pair->EdgeInner->Other = pair->EdgeOpposite;
		pair->EdgeOpposite->Other = pair->EdgeInner;
	}
}

/// <summary>
/// Compares the specified face with the specified compareTo face 
/// to see if they have a shared edge.
/// </summary>
/// <returns>
/// Returns true if the faces border an edge, otherwise false.
/// </returns>
bool HE_Face::HasSharedEdge(HE_Face* face, HE_Face* compareTo)
{
	HE_Pair edgePair;
	return HasSharedEdge(face, compareTo, &edgePair);
}

/// <summary>
/// Compares the specified face with the specified compareTo face 
/// to see if they have a shared edge.
/// </summary>
/// <param name="connectionPoints">
/// Outputs the shared edge.
/// </param>
/// <returns>
/// Returns true if the faces border an edge, otherwise false.
/// </returns>
bool HE_Face::HasSharedEdge(HE_Face* face, HE_Face* compareTo, HE_Pair* pair)
{
	bool isConnected = false; // I try to write optimistic code!
	HE_Edge* edgeInner;
	HE_Edge* edgeOpposite;
	HE_Vertex vertInner, vertOpposite;
	int sharedVertsCount = 0; // The number of shared verticies the faces share. 
							  // A primitive only has 2 shared verticies.

	pair = nullptr;

	//TODO: topology for triangles has only 1 shared edge so only have to worry about outputting the one shared edge

	edgeInner = face->Edge; // Begin the traversal of the edges inside the face.

	do
	{
		// Traverse the face to compare with for one edge that has two verticies that are shared.
		edgeOpposite = compareTo->Edge;
		vertInner = *edgeInner->Vertex;

		if (edgeInner->Other == nullptr)
		{
			do
			{
				vertOpposite = *edgeOpposite->Vertex;

				if (edgeOpposite->Other == nullptr)
				{

					if ((vertInner.Attributes->Vertex == vertOpposite.Attributes->Vertex)
						&& (vertInner.Edge->Vertex->Attributes->Vertex == vertOpposite.Edge->Vertex->Attributes->Vertex))
					{
						if (++sharedVertsCount == 2)
						{
							// There are two verticies in both the faces that are shared.
							// We know that edgeInner is shared with edgeOpposite.
							// These two edges can later be connected with a call to ConnectAdjacent().
							isConnected = true;

							HE_Pair* pairOut = new HE_Pair();
							pairOut->EdgeInner = edgeInner;
							pairOut->EdgeOpposite = edgeOpposite;
							pair = pairOut;
						}
					}
				}

				edgeOpposite = edgeOpposite->Next;

			} while ((edgeOpposite != compareTo->Edge) && !isConnected);
		}


		edgeInner = edgeInner->Next;
	} while ((edgeInner != face->Edge) && !isConnected);


	return isConnected;
}

std::vector<HE_Vertex>* HE_Face::GetVertexes()
{
	std::vector<HE_Vertex>* verticiesList = new std::vector<HE_Vertex>();
	HE_Edge* edge = Edge;

	// Traverse the edges of the Face.
	while (edge != Edge)
	{
		verticiesList->push_back(*edge->Vertex);

		edge = edge->Next;
	}
	return verticiesList;
}

std::vector<Vector3>* HE_Face::GetVerticies()
{
	std::vector<Vector3>* verticiesList = new std::vector<Vector3>();
	HE_Edge* edge = Edge;

	// Traverse the edges of the Face.
	do
	{
		verticiesList->push_back(edge->Vertex->Attributes->Vertex);

		edge = edge->Next;
	} while (edge != Edge);

	return verticiesList;
}

std::vector<Vector3>* HE_Face::GetNormals()
{
	std::vector<Vector3>* normalsList = new std::vector<Vector3>();
	HE_Edge* edge = Edge;

	// Traverse the edges of the Face.
	do
	{
		normalsList->push_back(edge->Vertex->Attributes->Normal);

		edge = edge->Next;
	} while (edge != Edge);

	return normalsList;
}

/// <summary>
/// Get the faces which border the half-edge.
/// </summary>
void HE_Face::GetAdjacentFaces(HE_Face* face1, HE_Face* face2)
{
	face1 = Edge->Face;
	face2 = Edge->Other->Face;
}

std::vector<HE_Face>* HE_Face::GetAdjacentFaces()
{
	std::vector<HE_Face>* facesList = new std::vector<HE_Face>();
	if (Edge->Face != nullptr)
	{
		facesList->push_back(*Edge->Face);
	}
	if (Edge->Other->Face != nullptr)
	{
		facesList->push_back(*Edge->Other->Face);
	}
	return facesList;
}

std::vector<HE_Face>* HE_Face::GetNearestFaces()
{
	std::vector<HE_Face>* nearestFaces2 = new std::vector<HE_Face>();
	std::vector<HE_Face*>* nearestFaces = new std::vector<HE_Face*>();
	HE_Edge* edgeInner = Edge;
	HE_Edge* edgePrev = Edge;
	int faces = 0;

	// Traverse the edge loop of the current face.
	do
	{
		if (edgeInner->Other != nullptr)
		{
            HE_Face* face = edgeInner->Other->Face;
            std::vector<HE_Face*>::iterator it = std::find(nearestFaces->begin(), nearestFaces->end(), face);

			if (it == nearestFaces->end())
			{
				nearestFaces->push_back(edgeInner->Other->Face);
                nearestFaces2->push_back(*edgeInner->Other->Face);
				faces++;
			}
		}

		edgePrev = edgeInner;
		edgeInner = edgeInner->Next;

	} while (edgeInner != Edge);

	return nearestFaces2;
}

/// <summary>
/// Get all the edges around the current Face.
/// </summary>
/// <returns></returns>
std::vector<HE_Edge>* HE_Face::GetEdges()
{
	std::vector<HE_Edge>* edgesList = new std::vector<HE_Edge>();
	HE_Face* face = this;
	HE_Edge* edge = face->Edge;

	do
	{
		edgesList->push_back(*edge);

		edge = edge->Next;

	} while (edge != face->Edge);

	return edgesList;
}