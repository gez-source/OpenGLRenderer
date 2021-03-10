#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Nullable.h"
#include "Vector3.h"
#include "ColourRGBA.h"

class Triangle // TODO: This is a TEST for just one type of polygon.
{
public:
	Vector3 positionXStep;
	Vector3 positionYStep;

	Vector4 colourXStep;
	Vector4 colourYStep;

	Vector3 normalXStep;
	Vector3 normalYStep;

	Vector2 texCoordXStep;
	Vector2 texCoordYStep;

	Vector3 Origin;

	Vector3 vertex00;
	Vector3 vertex01;
	Vector3 vertex10;

	Nullable<Vector3> normal00;
	Nullable<Vector3> normal01;
	Nullable<Vector3> normal10;

	Nullable<Vector4> colour00;
	Nullable<Vector4> colour01;
	Nullable<Vector4> colour10;

	Nullable<Vector2> texCoord00;
	Nullable<Vector2> texCoord01;
	Nullable<Vector2> texCoord10;

	Nullable<ColourRGBA> faceColour;

	bool HasNormalPerVertex = false;
	bool HasColourPerVertex = false;
	bool HasTexCoordPerVertex = false;

	bool AutocalcNormals = false;

	Triangle();

	Triangle(Vector3 t0, Vector3 t1, Vector3 t2);

	~Triangle();

	static Triangle* Clone(Triangle* t); // BUG:HACK: Cloning triangle because coordinates seem to be weirdly affected by transformations


	void Grad();

	Vector3 TriangleLerp(Vector2 uv);

	Vector3 TriangleVertexLerp(Vector2 uv);

	Vector3 TriangleNormalLerp(Vector2 uv); // if only!

	Vector4 TriangleColourLerp(Vector2 uv); // if only!

	bool HasVertexNormals();

	bool HasVertexColours();

	bool HasVertexTexCoords();

	Vector3 CenterOfTriangle();

	Vector3 GetNormal(Vector3 vertexPosition);

	Vector3 SurfaceNormal();

	Vector3 SurfaceNormalFront(); // Front face normal.
	Vector3 SurfaceNormalBack(); // Back face normal. (pointing in opposite direction)

	void SwapVectors();

	float GetArea();
	int GetHandednessI();
	bool GetHandedness();

	static float TriangleAreaTimesTwo(Vector3 b, Vector3 c, Vector3 m);

	static float TriangleAreaTimesTwo(Vector2 b, Vector2 c, Vector2 m);
};


#endif
