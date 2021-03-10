#include "Triangle.h"

Triangle::Triangle() : 
	positionXStep(Vector3::Zero),
	positionYStep(Vector3::Zero),
	colourXStep(Vector4::Zero),
	colourYStep(Vector4::Zero),
	normalXStep(Vector3::Zero),
	normalYStep(Vector3::Zero),
	texCoordXStep(Vector2::Zero),
	texCoordYStep(Vector2::Zero),
	Origin(Vector3::Zero),
	vertex00(Vector3::Zero),
	vertex01(Vector3::Zero),
	vertex10(Vector3::Zero),
	normal00(Nullable<Vector3>()),
	normal01(Nullable<Vector3>()),
	normal10(Nullable<Vector3>()),
	colour00(Nullable<Vector4>()),
	colour01(Nullable<Vector4>()),
	colour10(Nullable<Vector4>()),
	texCoord00(Nullable<Vector2>()),
	texCoord01(Nullable<Vector2>()),
	texCoord10(Nullable<Vector2>()),
	faceColour(Nullable<ColourRGBA>())
{
	// Nothing to do here.
}

Triangle::Triangle(Vector3 t0, Vector3 t1, Vector3 t2) :
	positionXStep(Vector3::Zero),
	positionYStep(Vector3::Zero),
	colourXStep(Vector4::Zero),
	colourYStep(Vector4::Zero),
	normalXStep(Vector3::Zero),
	normalYStep(Vector3::Zero),
	texCoordXStep(Vector2::Zero),
	texCoordYStep(Vector2::Zero),
	Origin(Vector3::Zero),
	vertex00(Vector3::Zero),
	vertex01(Vector3::Zero),
	vertex10(Vector3::Zero),
	normal00(Nullable<Vector3>()),
	normal01(Nullable<Vector3>()),
	normal10(Nullable<Vector3>()),
	colour00(Nullable<Vector4>()),
	colour01(Nullable<Vector4>()),
	colour10(Nullable<Vector4>()),
	texCoord00(Nullable<Vector2>()),
	texCoord01(Nullable<Vector2>()),
	texCoord10(Nullable<Vector2>()),
	faceColour(Nullable<ColourRGBA>())
{
	vertex00 = t0;
	vertex01 = t1;
	vertex10 = t2;
}

Triangle::~Triangle()
{

}

Triangle* Triangle::Clone(Triangle* t) // BUG:HACK: Cloning triangle because coordinates seem to be weirdly affected by transformations
{
	Triangle* c = new Triangle(Vector3(t->vertex00.x, t->vertex00.y, t->vertex00.z),
		Vector3(t->vertex01.x, t->vertex01.y, t->vertex01.z),
		Vector3(t->vertex10.x, t->vertex10.y, t->vertex10.z));

	if (t->HasVertexNormals())
	{

		c->normal00 = Vector3(t->normal00.get().x, t->normal00.get().y, t->normal00.get().z);
		c->normal01 = Vector3(t->normal01.get().x, t->normal01.get().y, t->normal01.get().z);
		c->normal10 = Vector3(t->normal10.get().x, t->normal10.get().y, t->normal10.get().z);
	}


	c->colour00 = t->colour00;
	c->colour01 = t->colour01;
	c->colour10 = t->colour10;

	c->texCoord00 = t->texCoord00;
	c->texCoord01 = t->texCoord01;
	c->texCoord10 = t->texCoord10;

	return c;
}


void Triangle::Grad()
{
	float minToMaxY = vertex00.y - vertex10.y;
	float midToMaxY = vertex01.y - vertex10.y;

	float minToMaxX = vertex00.x - vertex10.x;
	float midToMaxX = vertex01.x - vertex10.x;

	float oneOverdX = 1.0f / ((midToMaxX * minToMaxY) - (minToMaxX * midToMaxY));

	float oneOverdY = -oneOverdX;

	// Setup gradients for vertex attributes

	Vector3 v0 = vertex00;
	Vector3 v1 = vertex01;
	Vector3 v2 = vertex10;

	positionXStep =
		(((v1 - v2) * minToMaxY) - ((v0 - v2) * midToMaxY)) * oneOverdX;

	positionYStep =
		(((v1 - v2) * minToMaxX) - ((v0 - v2) * midToMaxX)) * oneOverdY;


	if (HasVertexColours())
	{
		Vector4 m0 = colour00.get();
		Vector4 m1 = colour01.get();
		Vector4 m2 = colour10.get();

		colourXStep =
			(((m1 - m2) * minToMaxY) - ((m0 - m2) * midToMaxY)) * oneOverdX;

		colourYStep =
			(((m1 - m2) * minToMaxX) - ((m0 - m2) * midToMaxX)) * oneOverdY;
	}

	if (HasVertexNormals())
	{
		Vector3 m0 = normal00.get();
		Vector3 m1 = normal01.get();
		Vector3 m2 = normal10.get();

		normalXStep =
			(((m1 - m2) * minToMaxY) - ((m0 - m2) * midToMaxY)) * oneOverdX;

		normalYStep =
			(((m1 - m2) * minToMaxX) - ((m0 - m2) * midToMaxX)) * oneOverdY;
	}

	if (HasVertexTexCoords())
	{
		Vector2 m0 = texCoord00.get();
		Vector2 m1 = texCoord01.get();
		Vector2 m2 = texCoord10.get();

		texCoordXStep =
			(((m1 - m2) * minToMaxY) - ((m0 - m2) * midToMaxY)) * oneOverdX;

		texCoordYStep =
			(((m1 - m2) * minToMaxX) - ((m0 - m2) * midToMaxX)) * oneOverdY;
	}
}

Vector3 Triangle::TriangleLerp(Vector2 uv)
{
	Vector3 result = Vector3::TriangleLerp(vertex00, vertex01, vertex10, uv.x, uv.y);
	return result;
}

Vector3 Triangle::TriangleVertexLerp(Vector2 uv)
{
	Vector3 result = Vector3::TriangleLerp(vertex00, vertex01, vertex10, uv.x, uv.y);
	return result;
}

Vector3 Triangle::TriangleNormalLerp(Vector2 uv) // if only!
{
	Vector3 result = Vector3::TriangleLerp(normal00.get(), normal01.get(), normal10.get(), uv.x, uv.y);
	return result;
}

Vector4 Triangle::TriangleColourLerp(Vector2 uv) // if only!
{
	Vector4 result = Vector4::TriangleLerp(colour00.get(), colour01.get(), colour10.get(), uv.x, uv.y);
	return result;
}

bool Triangle::HasVertexNormals()
{
	return normal00.is_set() && normal01.is_set() && normal10.is_set();
}

bool Triangle::HasVertexColours()
{
	return colour00.is_set() && colour01.is_set() && colour10.is_set();
}

bool Triangle::HasVertexTexCoords()
{
	return texCoord00.is_set() && texCoord01.is_set() && texCoord10.is_set();
}

Vector3 Triangle::CenterOfTriangle()
{
	return (vertex00 + vertex01 + vertex10) / 3;
}

Vector3 Triangle::GetNormal(Vector3 vertexPosition)
{
	Vector3 result = (vertexPosition - Origin).Normalised();
	return result;
}

Vector3 Triangle::SurfaceNormal()
{
	Vector3 u = vertex01 - vertex00;
	Vector3 v = vertex10 - vertex00;
	Vector3 normal = Vector3::Cross(u, v);
	Vector3 result = normal.Normalised();
	return result;
}

Vector3 Triangle::SurfaceNormalFront() // Front face normal.
{
	Vector3 edge1 = vertex01 - vertex00; //TODO: Refactor using HE_Edge :)
	Vector3 edge2 = vertex10 - vertex01;
	Vector3 result = Vector3::Cross(edge1, edge2).Normalised();
	return result;
}

Vector3 Triangle::SurfaceNormalBack() // Back face normal. (pointing in opposite direction)
{
	Vector3 edge1 = vertex01 - vertex10; // Opposite winding order, or opposite edge loop.
	Vector3 edge2 = vertex00 - vertex01;
	Vector3 result = Vector3::Cross(edge1, edge2).Normalised();
	return result;
}

void Triangle::SwapVectors()
{
	// Sort vertex atrributes by y-value.
	Vector3 tmp;
	Vector4 col;
	Vector3 norm;
	Vector2 tex;

	bool vc = HasVertexColours();
	bool vn = HasVertexNormals();
	bool vt = HasVertexTexCoords();

	if (vertex10.y < vertex01.y)
	{
		tmp = vertex10;
		vertex10 = vertex01;
		vertex01 = tmp;

		if (vc)
		{
			col = colour10.get();
			colour10 = colour01;
			colour01 = col;
		}

		if (vn)
		{
			norm = normal10.get();
			normal10 = normal01;
			normal01 = norm;
		}

		if (vt)
		{
			tex = texCoord10.get();
			texCoord10 = texCoord01;
			texCoord01 = tex;
		}
	}
	if (vertex01.y < vertex00.y)
	{
		tmp = vertex01;
		vertex01 = vertex00;
		vertex00 = tmp;

		if (vc)
		{
			col = colour01.get();
			colour01 = colour00;
			colour00 = col;
		}

		if (vn)
		{
			norm = normal01.get();
			normal01 = normal00;
			normal00 = norm;
		}

		if (vt)
		{
			tex = texCoord01.get();
			texCoord01 = texCoord00;
			texCoord00 = tex;
		}
	}
	if (vertex10.y < vertex01.y)
	{
		tmp = vertex10;
		vertex10 = vertex01;
		vertex01 = tmp;

		if (vc)
		{
			col = colour10.get();
			colour10 = colour01;
			colour01 = col;
		}

		if (vn)
		{
			norm = normal10.get();
			normal10 = normal01;
			normal01 = norm;
		}

		if (vt)
		{
			tex = texCoord10.get();
			texCoord10 = texCoord01;
			texCoord01 = tex;
		}
	}
}

float Triangle::GetArea()
{
	return TriangleAreaTimesTwo(vertex10.xy(), vertex01.xy(), vertex00.xy());
}
int Triangle::GetHandednessI()
{
	return GetArea() >= 0 ? 1 : 0;
}
bool Triangle::GetHandedness()
{
	return GetArea() >= 0;
}

float Triangle::TriangleAreaTimesTwo(Vector3 b, Vector3 c, Vector3 m)
{
	Vector3 d1 = b - m;
	Vector3 d2 = c - m;
	return 0.5f * Vector3::Crossf(d1.xy(), d2.xy());
}

float Triangle::TriangleAreaTimesTwo(Vector2 b, Vector2 c, Vector2 m)
{
	Vector2 d1 = b - m;
	Vector2 d2 = c - m;

	return 0.5f * Vector2::Cross(d1, d2);
}
