#ifndef QUAD_H
#define QUAD_H

#include "Nullable.h"
#include "Vector3.h"
#include "ColourRGBA.h"

class Quad
{
public:
	Vector3& vertex00;
	Vector3& vertex01;
	Vector3& vertex10;
	Vector3& vertex11;

	Nullable<Vector3> normal00;
	Nullable<Vector3> normal01;
	Nullable<Vector3> normal10;
	Nullable<Vector3> normal11;

	Nullable<Vector4> colour00;
	Nullable<Vector4> colour01;
	Nullable<Vector4> colour10;
	Nullable<Vector4> colour11;

	Nullable<Vector2> texCoord00;
	Nullable<Vector2> texCoord01;
	Nullable<Vector2> texCoord10;
	Nullable<Vector2> texCoord11;

	bool HasNormalPerVertex = false;
	bool HasColourPerVertex = false;
	bool HasTexCoordPerVertex = false;
	bool AutocalcNormals = false;

	Nullable<ColourRGBA> faceColour;

	bool HasVertexNormals() const;

	bool HasVertexColours() const;

	Quad();

	Quad(Vector3 q0, Vector3 q1, Vector3 q2, Vector3 q3);
};


#endif
