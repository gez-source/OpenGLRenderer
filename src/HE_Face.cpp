#include "HE_Face.h"
#include "IndexedTriangleSet.h"


HE_Face::HE_Face()
{
	t = new Triangle();
	Attributes = new FaceAttributes();
}

HE_Face::~HE_Face()
{
	delete t;
	delete Attributes;
}

/// <summary>
/// Applies a transformation matrix to all verticies in the face.
/// </summary>
/// <param name="tranform"></param>
void HE_Face::Transform(Matrix4 tranform)
{
	Vector4 v0 = Vector4(t->vertex00, 1.0f);
	Vector4 v1 = Vector4(t->vertex01, 1.0f);
	Vector4 v2 = Vector4(t->vertex10, 1.0f);

	t->vertex00 = Matrix4::TransformVector(tranform, v0).xyz();
	t->vertex01 = Matrix4::TransformVector(tranform, v1).xyz();
	t->vertex10 = Matrix4::TransformVector(tranform, v2).xyz();

	//TODO: also update Vertex data in edge loop.
}

Vector3 HE_Face::CenterOfMass()
{
	return t->CenterOfTriangle();
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


Triangle HE_Face::GetPrimitive()
{
	Triangle* clone = Triangle::Clone(t);

	return *clone;
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

	if (t->HasVertexNormals())
	{
		Vector3 n0 = t->normal00.get();
		Vector3 n1 = t->normal01.get();
		Vector3 n2 = t->normal10.get();
		normalsList->push_back(n0);
		normalsList->push_back(n1);
		normalsList->push_back(n2);
	}

	return normalsList;
}

HE_Face* HE_Face::GetFace(IndexedTriangleSet* triangleSet, int faceIndex)
{
	int vertexSet = 3;
	HE_Face* face = new HE_Face();

	face->FaceIndex = faceIndex;
	face->VertexCount = vertexSet;

	face->Attributes->Polygon = PolygonType::PT_Triangles;

	face->GetFace(triangleSet, faceIndex, vertexSet);

	return face;
}

//TODO: Refactor

void HE_Face::GetFace(IndexedTriangleSet* triangleSet, int faceIndex, int vertexSet)
{
	int polyIndex = faceIndex * vertexSet;

	Vector3 vertex00 = triangleSet->Verticies->at(triangleSet->Indicies->at(polyIndex + 0));
	Vector3 vertex01 = triangleSet->Verticies->at(triangleSet->Indicies->at(polyIndex + 1));
	Vector3 vertex10 = triangleSet->Verticies->at(triangleSet->Indicies->at(polyIndex + 2));

	int faceCount = ((triangleSet->Indicies->size()) / vertexSet);

	t = new Triangle(vertex00, vertex01, vertex10);

	if (triangleSet->FaceColours != nullptr && faceIndex < faceCount - 1)
	{
		t->faceColour = triangleSet->FaceColours->at(faceIndex);
	}
	else
	{
		if (triangleSet->shapeColour.is_set())
		{
			t->faceColour = triangleSet->shapeColour.get();
		}
	}

	if (triangleSet->Normals != nullptr && triangleSet->Normals->size() > 0)
	{
		t->HasNormalPerVertex = true;

		if (triangleSet->NormalIndicies != nullptr)
		{
			t->normal00 = triangleSet->Normals->at(triangleSet->NormalIndicies->at(polyIndex + 0));
			t->normal01 = triangleSet->Normals->at(triangleSet->NormalIndicies->at(polyIndex + 1));
			t->normal10 = triangleSet->Normals->at(triangleSet->NormalIndicies->at(polyIndex + 2));
		}
		else
		{
			t->normal00 = triangleSet->Normals->at(triangleSet->Indicies->at(polyIndex + 0));
			t->normal01 = triangleSet->Normals->at(triangleSet->Indicies->at(polyIndex + 1));
			t->normal10 = triangleSet->Normals->at(triangleSet->Indicies->at(polyIndex + 2));
		}

	}
	else
	{
		t->AutocalcNormals = true;
	}

	if (triangleSet->VertexColours != nullptr && triangleSet->VertexColours->size() > 0)
	{
		t->HasColourPerVertex = true;

		t->colour00 = (triangleSet->VertexColours->at(triangleSet->Indicies->at(polyIndex + 0))).ToVec4();
		t->colour01 = (triangleSet->VertexColours->at(triangleSet->Indicies->at(polyIndex + 1))).ToVec4();
		t->colour10 = (triangleSet->VertexColours->at(triangleSet->Indicies->at(polyIndex + 2))).ToVec4();
	}

	if (triangleSet->TextCoords != nullptr && triangleSet->TextCoords->size() > 0)
	{
		t->HasTexCoordPerVertex = true;

		if (triangleSet->TexCoordIndicies != nullptr)
		{
			t->texCoord00 = triangleSet->TextCoords->at(triangleSet->TexCoordIndicies->at(polyIndex + 0));
			t->texCoord01 = triangleSet->TextCoords->at(triangleSet->TexCoordIndicies->at(polyIndex + 1));
			t->texCoord10 = triangleSet->TextCoords->at(triangleSet->TexCoordIndicies->at(polyIndex + 2));
		}
		else
		{
			t->texCoord00 = triangleSet->TextCoords->at(triangleSet->Indicies->at(polyIndex + 0));
			t->texCoord01 = triangleSet->TextCoords->at(triangleSet->Indicies->at(polyIndex + 1));
			t->texCoord10 = triangleSet->TextCoords->at(triangleSet->Indicies->at(polyIndex + 2));
		}
	}

	// Create all the half-edges and adjacencies for the quad.
	GetEdges(polyIndex, vertex00, vertex01, vertex10, t);
}

//TODO: Refactor
void HE_Face::GetEdges(int polyIndex, Vector3 v00, Vector3 v01, Vector3 v10, Triangle* t)
{
	// Imbue the knowledge of surrounding faces and edges to a vertex.

	// Which edges use a vertex? - Determined here.

	// Which faces use a vertex? - Determined by the Push() operation in the HalfEdgeMesh structure.

	HE_Edge* edge_v00_0 = new HE_Edge();
	HE_Edge* edge_v00_1 = new HE_Edge();
	HE_Edge* edge_v01_0 = new HE_Edge();
	HE_Edge* edge_v01_1 = new HE_Edge();
	HE_Edge* edge_v10_0 = new HE_Edge();
	HE_Edge* edge_v10_1 = new HE_Edge();

	HE_Vertex* v00_0 = new HE_Vertex();
	HE_Vertex* v01_1 = new HE_Vertex();
	HE_Vertex* v10_2 = new HE_Vertex();

	//v00_0->Attributes->Vertex = v00;
	v00_0->Attributes->Normal = t->normal00.get();
	v00_0->Attributes->TextureCoordinate = t->texCoord00.get();
	v00_0->Attributes->Colour = t->colour00.get();

	//v01_1->Attributes->Vertex = v01;
	v01_1->Attributes->Normal = t->normal01.get();
	v01_1->Attributes->TextureCoordinate = t->texCoord01.get();
	v01_1->Attributes->Colour = t->colour01.get();

	//v10_2->Attributes->Vertex = v10;
	v10_2->Attributes->Normal = t->normal10.get();
	v10_2->Attributes->TextureCoordinate = t->texCoord10.get();
	v10_2->Attributes->Colour = t->colour10.get();


	//TODO:CHECK: verticies may need to be ordered counter-clockwise. Instead they are presently ordered clockwise Opps.

	edge_v00_0->Index = 0;
	edge_v00_1->Index = 1;
	edge_v01_0->Index = 2;
	edge_v01_1->Index = 3;
	edge_v10_0->Index = 4;
	edge_v10_1->Index = 5;

	// Associate the current face to each edge.
	edge_v00_0->Face = this;
	edge_v00_1->Face = this;
	edge_v01_0->Face = this;
	edge_v01_1->Face = this;
	edge_v10_0->Face = this;
	edge_v10_1->Face = this;

	// Associate an edge to its vertex.
	edge_v00_0->Vertex = v00_0;
	edge_v00_1->Vertex = v00_0;
	edge_v01_0->Vertex = v01_1;
	edge_v01_1->Vertex = v01_1;
	edge_v10_0->Vertex = v10_2;
	edge_v10_1->Vertex = v10_2;

	// Set the primary edges of each vertex.
	v00_0->Edge = edge_v00_0;
	v01_1->Edge = edge_v01_0;
	v10_2->Edge = edge_v10_0;

	// Set vertex indicies for each vertex.
	v00_0->Index = polyIndex + 0;
	v01_1->Index = polyIndex + 1;
	v10_2->Index = polyIndex + 2;


	// Associate the each edge to the next one in the chain.
	edge_v00_0->Next = edge_v01_0;
	edge_v01_0->Next = edge_v10_0;

	// Set the tail of the list to point back to first vertex-edge.
	// Making the edge list a circularly linked list.
	edge_v10_0->Next = edge_v00_0; // Loop. This is more useful than a null terminator.

								   // Make the edges doubly-linked.
	edge_v00_0->Previous = edge_v10_0; // Loop.
	edge_v01_0->Previous = edge_v00_0;
	edge_v10_0->Previous = edge_v01_0;

	// Set the first edge of the current face to point to the first vertex-edge.
	Edge = edge_v00_0;
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

std::vector<Triangle>* HE_Face::GetTriangles()
{
	// Input requires a valid set of triangle polygons.
	// 3 edges per poly
	std::vector<Triangle>* trianglesList = new std::vector<Triangle>();
	std::vector<HE_Edge>* edgesList = new std::vector<HE_Edge>();
	HE_Edge* edge;
	int i;
	Triangle* tri;
	int vertexSet = 3;

	if (edgesList->size() > 3)
	{
		// TODO: Subdivide the current face into triangles if it has more than 3 edges
	}

	for (i = 0; i < edgesList->size(); i++)
	{
		edge = &edgesList->at(i);

		if ((i % vertexSet == vertexSet) && i != 0)
		{
			tri = new Triangle();
			tri->vertex00 = edgesList->at(i - 2).Vertex->Attributes->Vertex;
			tri->vertex01 = edgesList->at(i - 1).Vertex->Attributes->Vertex;
			tri->vertex10 = edgesList->at(i - 0).Vertex->Attributes->Vertex;
			trianglesList->push_back(*tri);
		}
	}

	return trianglesList;
}
