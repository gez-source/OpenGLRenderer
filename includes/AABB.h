#ifndef AABB_H
#define AABB_H

#include "Vector3.h"

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
	Vector3 HalfSize;

	/// <summary>
	/// The minimum of the bounding box.
	/// </summary>
	Vector3 Min;

	/// <summary>
	/// The maximum of the bounding box.
	/// </summary>
	Vector3 Max;

	AABB();
	AABB(Vector3 center, Vector3 halfSize);

	/// <summary>
	/// Get the center of the bounding box.
	/// </summary>
	Vector3 GetCenter();

	/// <summary>
	/// Get the size of the bounding box.
	/// </summary>
	Vector3 GetSize();

	/// <summary>
	/// Get the half size (radius vector) of the bounding box.
	/// </summary>
	Vector3 GetHalfSize();

	/// <summary>
	/// The minimum extent of the bounding box.
	/// </summary>
	Vector3 GetMin();

	/// <summary>
	/// The maximum extent of the bounding box.
	/// </summary>
	Vector3 GetMax();

	/// <summary>
	/// Clears the AABB.
	/// </summary>
	void Empty();

	/// <summary>
	/// Add a point to the axis-aligned bounding box to compute the AABB.
	/// </summary>
	void Add(Vector3 point);

	/// <summary>
	/// Add a bounding box to compute a greater AABB.
	/// </summary>
	void Add(AABB boundingBox);

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

	/// <summary>
	/// Draws lines for each plane of the bounding box.
	/// </summary>
	void Debug(ColourRGBA boundsColour = ColourRGBA(1.0f, 1.0f, 0.0f, 0.0f));
};

#endif
