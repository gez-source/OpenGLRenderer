#ifndef VERTEXATTRIBUTES_H
#define VERTEXATTRIBUTES_H

#include "Nullable.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

class VertexAttributes
{
public:
	Vector3 Vertex;

	Nullable<Vector3> Normal;
	Nullable<Vector4> Colour;
	Nullable<Vector2> TextureCoordinate;

	VertexAttributes():
		Normal(Nullable<Vector3>()),
		Colour(Nullable<Vector4>()),
		TextureCoordinate(Nullable<Vector2>()),
		Vertex(Vector3::Zero)
	{
		
	}
};

#endif
