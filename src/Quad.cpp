#include "Quad.h"

Quad::Quad() :
	vertex00(Vector3::Zero),
	vertex01(Vector3::Zero),
	vertex10(Vector3::Zero),
	vertex11(Vector3::Zero),
	normal00(Nullable<Vector3>()),
	normal01(Nullable<Vector3>()),
	normal10(Nullable<Vector3>()),
	normal11(Nullable<Vector3>()),
	colour00(Nullable<Vector4>()),
	colour01(Nullable<Vector4>()),
	colour10(Nullable<Vector4>()),
	colour11(Nullable<Vector4>()),
	texCoord00(Nullable<Vector2>()),
	texCoord01(Nullable<Vector2>()),
	texCoord10(Nullable<Vector2>()),
	texCoord11(Nullable<Vector2>()),
	faceColour(Nullable<ColourRGBA>())
{
	
}

Quad::Quad(Vector3 q0, Vector3 q1, Vector3 q2, Vector3 q3) :
	vertex00(Vector3::Zero),
	vertex01(Vector3::Zero),
	vertex10(Vector3::Zero),
	vertex11(Vector3::Zero),
	normal00(Nullable<Vector3>()),
	normal01(Nullable<Vector3>()),
	normal10(Nullable<Vector3>()),
	normal11(Nullable<Vector3>()),
	colour00(Nullable<Vector4>()),
	colour01(Nullable<Vector4>()),
	colour10(Nullable<Vector4>()),
	colour11(Nullable<Vector4>()),
	texCoord00(Nullable<Vector2>()),
	texCoord01(Nullable<Vector2>()),
	texCoord10(Nullable<Vector2>()),
	texCoord11(Nullable<Vector2>()),
	faceColour(Nullable<ColourRGBA>())
{
	vertex00 = q0;
	vertex01 = q1;
	vertex10 = q2;
	vertex11 = q3;
}

bool Quad::HasVertexNormals() const
{
	return normal00.is_set() && normal01.is_set() && normal10.is_set() && normal11.is_set();
}

bool Quad::HasVertexColours() const
{
	return colour00.is_set() && colour01.is_set() && colour10.is_set() && colour11.is_set();
}

