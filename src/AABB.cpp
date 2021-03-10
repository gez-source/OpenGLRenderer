#include "AABB.h"

AABB::AABB(Vector3 center, float halfSize)
{
	Center = center;
	HalfSize = halfSize;
}

/// <summary>
/// The minimum extent of the bounding box.
/// </summary>
Vector3 AABB::Min()
{
	return Vector3(Center.x - HalfSize, Center.y - HalfSize, Center.z - HalfSize);
}

/// <summary>
/// The maximum extent of the bounding box.
/// </summary>
Vector3 AABB::Max()
{
	return Vector3(Center.x + HalfSize, Center.y + HalfSize, Center.z + HalfSize);
}

///// <summary>
///// Test if the specified polygon intersects this bounding box.
///// </summary>
//bool AABB::Intersects(HE_Face* polygon)
//{
//	//TODO:HACK: Dirty hack just calculating if the center of the polygon is within the box. Better to do a proper intersection test. For now this is fine.
//
//	Vector3 polyCenter = polygon->CenterOfMass();
//
//	Vector3 distance = Center - polyCenter;
//
//	return (abs(distance.x) <= HalfSize)
//		&& (abs(distance.y) <= HalfSize)
//		&& (abs(distance.z) <= HalfSize);
//}

/// <summary>
/// Test if the specified point intersects this bounding box.
/// </summary>
bool AABB::Intersects(Vector3 point)
{
	Vector3 distance = Center - point;

	return (abs(distance.x) <= HalfSize)
		&& (abs(distance.y) <= HalfSize)
		&& (abs(distance.z) <= HalfSize);
}

/// <summary>
/// Test if the specified point intersects this bounding box.
/// </summary>
bool AABB::Intersects(Vector2 point)
{
	Vector2 distance(Center.x - point.x, Center.y - point.y);

	return (abs(distance.x) <= HalfSize) && (abs(distance.y) <= HalfSize);
}

/// <summary>
/// Test if the specified bounding box intersects this bounding box.
/// </summary>
bool AABB::Intersects(AABB* other)
{
	Vector3 min(Center.x - HalfSize, Center.y - HalfSize, Center.z - HalfSize);
	Vector3 max(Center.x + HalfSize, Center.y + HalfSize, Center.z + HalfSize);

	if (min.x >= other->Max().x) return false;
	if (max.x <= other->Min().x) return false;

	if (min.y >= other->Max().y) return false;
	if (max.y <= other->Min().y) return false;

	if (min.z >= other->Max().z) return false;
	if (max.z <= other->Min().z) return false;

	// There is an Overlap on both axes, so there is an intersection.
	return true;
}

//void AABB::Debug2D(RenderingContext renderer, ColourRGBA& boundsColour, ColourRGBA& centerColour)
//{
//	// Debug the center point.
//	//renderer.DebugPoint(Center, centerColour, 1);
//
//	Vector3 c = Center;
//	float s = HalfSize;
//	// Debug the bounds of the AABB, drawing the edges of the box.
//
//	// FRONT PLANE
//	// Top.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z + s), Vector3(c.x + s, c.y + s, c.z + s), boundsColour);
//
//	// Bottom.
//	renderer.DebugLine(Vector3(c.x - s, c.y - s, c.z + s), Vector3(c.x + s, c.y - s, c.z + s), boundsColour);
//
//	// Left.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z + s), Vector3(c.x - s, c.y - s, c.z + s), boundsColour);
//
//	// Right.
//	renderer.DebugLine(Vector3(c.x + s, c.y + s, c.z + s), Vector3(c.x + s, c.y - s, c.z + s), boundsColour);
//}
//
//void AABB::Debug(RenderingContext renderer, ColourRGBA& boundsColour, ColourRGBA& centerColour)
//{
//	// Debug the center point.
//	renderer.DebugPoint(Center, centerColour, 1);
//
//	Vector3 c = Center;
//	float s = HalfSize;
//	// Debug the bounds of the AABB, drawing the edges of the box.
//
//	// FRONT PLANE
//	// Top.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z + s), Vector3(c.x + s, c.y + s, c.z + s), boundsColour);
//
//	// Bottom.
//	renderer.DebugLine(Vector3(c.x - s, c.y - s, c.z + s), Vector3(c.x + s, c.y - s, c.z + s), boundsColour);
//
//	// Left.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z + s), Vector3(c.x - s, c.y - s, c.z + s), boundsColour);
//
//	// Right.
//	renderer.DebugLine(Vector3(c.x + s, c.y + s, c.z + s), Vector3(c.x + s, c.y - s, c.z + s), boundsColour);
//
//	//  DEBUG Z-axis
//
//	// BACK PLANE
//	// Top.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z - s), Vector3(c.x + s, c.y + s, c.z - s), boundsColour);
//
//	// Bottom.
//	renderer.DebugLine(Vector3(c.x - s, c.y - s, c.z - s), Vector3(c.x + s, c.y - s, c.z - s), boundsColour);
//
//	// Left.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z - s), Vector3(c.x - s, c.y - s, c.z - s), boundsColour);
//
//	// Right.
//	renderer.DebugLine(Vector3(c.x + s, c.y + s, c.z - s), Vector3(c.x + s, c.y - s, c.z - s), boundsColour);
//
//	// LEFT PLANE
//	// top left edge front to back.
//	renderer.DebugLine(Vector3(c.x - s, c.y + s, c.z + s), Vector3(c.x - s, c.y + s, c.z - s), boundsColour);
//
//	// bottom left edge front to back.
//	renderer.DebugLine(Vector3(c.x - s, c.y - s, c.z + s), Vector3(c.x - s, c.y - s, c.z - s), boundsColour);
//
//	// RIGHT PLANE
//	//top right edge front to back.
//	renderer.DebugLine(Vector3(c.x + s, c.y + s, c.z + s), Vector3(c.x + s, c.y + s, c.z - s), boundsColour);
//
//	// bottom right edge front to back.
//	renderer.DebugLine(Vector3(c.x + s, c.y - s, c.z + s), Vector3(c.x + s, c.y - s, c.z - s), boundsColour);
//
//
//}