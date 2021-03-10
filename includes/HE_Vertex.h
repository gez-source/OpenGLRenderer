#ifndef HE_VERTEX_H
#define HE_VERTEX_H

#include "HE_Edge.h"
#include "Vector3.h"
#include "VertexAttributes.h"

class Vector3;
class HE_Edge;
class VertexAttributes;

class HE_Vertex
{
public:
	int Index = -1;
	
	VertexAttributes* Attributes;

	/// One of the half-edges emantating from the vertex.
	HE_Edge* Edge;

	HE_Vertex();
	HE_Vertex(int index);
	HE_Vertex(int index, Vector3 vertexPosition);

	~HE_Vertex();

	bool Equals(HE_Vertex otherVertex) const;
};

#endif
