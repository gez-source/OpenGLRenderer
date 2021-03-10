#ifndef FACEATTRIBUTES_H
#define FACEATTRIBUTES_H

#include "Nullable.h"
#include "Enums.h"

class FaceAttributes
{
public:
	PolygonType Polygon;

	Vector3 Origin;

	Nullable<Vector4> faceColour;

	bool HasNormalPerVertex = false;
	bool HasColourPerVertex = false;

	bool AutocalcNormals = false;

	FaceAttributes() : 
		Polygon(PolygonType::PT_Triangles),
		Origin(Vector3::Zero),
		faceColour(Nullable<Vector4>())
	{

	}
};

#endif
