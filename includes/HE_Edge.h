#ifndef HE_EDGE_H
#define HE_EDGE_H

#include <string>
#include <sstream>
#include "HE_Vertex.h"
#include "HE_Face.h"

class HE_Vertex;
class HE_Edge;
class HE_Face;

class HE_Edge
{
public:

	int Index;

	/// Vertex at the end of the half-edge.
	HE_Vertex* Vertex;

	/// Oppositely oriented adjacent half-edge.
	HE_Edge* Other;

	/// Face the half-edge borders. 
	/// The current face the edge belongs to (not the adjacent face).
	HE_Face* Face;

	/// Next half-edge around the face.
	HE_Edge* Next;

	/// The previous half-edge on the same face. Just in the opposite winding.
	HE_Edge* Previous;

	HE_Edge();

	HE_Edge(HE_Vertex vertex);

	HE_Edge(HE_Vertex vertex, HE_Face face);

	HE_Edge(HE_Vertex vertex, HE_Face face, int edgeIndex);

	std::string GetName();

	/// Get the vertices which border this half-edge.
	void GetAdjacentVerticies(HE_Vertex start, HE_Vertex end);

	/// Get the faces which border this half-edge.
	void GetAdjacentFaces(HE_Face face1, HE_Face face2);
};

#endif
