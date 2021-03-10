#include "HE_Edge.h"

HE_Edge::HE_Edge() {}

HE_Edge::HE_Edge(HE_Vertex vertex)
{
	Vertex = &vertex;
}

HE_Edge::HE_Edge(HE_Vertex vertex, HE_Face face)
{
	Vertex = &vertex;
	Face = &face;
}

HE_Edge::HE_Edge(HE_Vertex vertex, HE_Face face, int edgeIndex)
{
	Vertex = &vertex;
	Face = &face;
	Index = edgeIndex;
}

std::string HE_Edge::GetName() // For testing.
{
	std::stringstream ss;
	ss << "Edge ";
	ss << Index;
	ss << " for F ";
	ss << Face->FaceIndex;
	ss << " V ";
	ss << Vertex->Index;
	return ss.str();
}

/// Get the vertices which border this half-edge.
void HE_Edge::GetAdjacentVerticies(HE_Vertex start, HE_Vertex end)
{
	start = *Vertex;
	end = *Other->Vertex;
}

/// Get the faces which border this half-edge.
void HE_Edge::GetAdjacentFaces(HE_Face face1, HE_Face face2)
{
	face1 = *Face;
	face2 = *Other->Face;
}
