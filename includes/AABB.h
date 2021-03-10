#ifndef AABB_H
#define AABB_H

#include "Vector3.h"
//#include "HE_Face.h"

//class HE_Face;

class AABB
{
public:
	/// <summary>
	/// The center of the bounding box.
	/// </summary>
	Vector3 Center;

	/// <summary>
	/// The half-size of the bounding box.
	/// </summary>
	float HalfSize;

	AABB(Vector3 center, float halfSize);

	/// <summary>
	/// The minimum extent of the bounding box.
	/// </summary>
	Vector3 Min();

	/// <summary>
	/// The maximum extent of the bounding box.
	/// </summary>
	Vector3 Max();

	/// <summary>
	/// Test if the specified polygon intersects this bounding box.
	/// </summary>
	//bool Intersects(HE_Face* polygon);

	/// <summary>
	/// Test if the specified point intersects this bounding box.
	/// </summary>
	bool Intersects(Vector3 point);

	/// <summary>
	/// Test if the specified point intersects this bounding box.
	/// </summary>
	bool Intersects(Vector2 point);

	/// <summary>
	/// Test if the specified bounding box intersects this bounding box.
	/// </summary>
	bool Intersects(AABB* other);
};

#endif
