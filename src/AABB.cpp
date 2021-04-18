#include "AABB.h"
#include "Debug.h"

AABB::AABB()
{
	Empty();
}

AABB::AABB(Vector3 center, Vector3 halfSize)
{
	Center = center;
	HalfSize = halfSize;

	Min = GetMin();
	Max = GetMax();
}

/// <summary>
/// Get the center of the bounding box.
/// </summary>
Vector3 AABB::GetCenter()
{
	return (Max + Min) / 2;
}

/// <summary>
/// Get the size of the bounding box.
/// </summary>
Vector3 AABB::GetSize()
{
	return Max - Min;
}

/// <summary>
/// Get the half size (radius vector) of the bounding box.
/// </summary>
Vector3 AABB::GetHalfSize()
{
	return GetSize() / 2; // == Max - Center.
}

/// <summary>
/// The minimum extent of the bounding box.
/// </summary>
Vector3 AABB::GetMin()
{
	return Vector3(Center.x - HalfSize.x, Center.y - HalfSize.y, Center.z - HalfSize.z);
}

/// <summary>
/// The maximum extent of the bounding box.
/// </summary>
Vector3 AABB::GetMax()
{
	return Vector3(Center.x + HalfSize.x, Center.y + HalfSize.y, Center.z + HalfSize.z);
}

/// <summary>
/// Clears the AABB.
/// </summary>
void AABB::Empty()
{
	Center = Vector3::Zero;
	HalfSize = Vector3::Zero;

	Min.x = Min.y = Min.z = FLT_MAX;
	Max.x = Max.y = Max.z = -FLT_MAX;
}

/// <summary>
/// Add a point to the axis-aligned bounding box to compute the AABB.
/// </summary>
void AABB::Add(Vector3 point)
{
	if (point.x < Min.x) Min.x = point.x;
	if (point.x > Max.x) Max.x = point.x;

	if (point.y < Min.y) Min.y = point.y;
	if (point.y > Max.y) Max.y = point.y;

	if (point.z < Min.z) Min.z = point.z;
	if (point.z > Max.z) Max.z = point.z;
}

/// <summary>
/// Add a bounding box to compute a greater AABB.
/// </summary>
void AABB::Add(AABB boundingBox)
{
	this->Add(boundingBox.Min);
	this->Add(boundingBox.Max);
	this->Add(boundingBox.Center);
}

/// <summary>
/// Test if the specified point intersects this bounding box.
/// </summary>
bool AABB::Intersects(Vector3 point)
{
	Vector3 distance = Center - point;

	return (abs(distance.x) <= HalfSize.x)
		&& (abs(distance.y) <= HalfSize.y)
		&& (abs(distance.z) <= HalfSize.z);
}

/// <summary>
/// Test if the specified point intersects this bounding box.
/// </summary>
bool AABB::Intersects(Vector2 point)
{
	Vector2 distance(Center.x - point.x, Center.y - point.y);

	return (abs(distance.x) <= HalfSize.x) && (abs(distance.y) <= HalfSize.y);
}

/// <summary>
/// Test if the specified bounding box intersects this bounding box.
/// </summary>
bool AABB::Intersects(AABB* other)
{
	//Vector3 min(Center.x - HalfSize.x, Center.y - HalfSize.y, Center.z - HalfSize.z);
	//Vector3 max(Center.x + HalfSize.x, Center.y + HalfSize.y, Center.z + HalfSize.z);
	Vector3 min = Min;
	Vector3 max = Max;

	if (min.x >= other->Max.x) return false;
	if (max.x <= other->Min.x) return false;

	if (min.y >= other->Max.y) return false;
	if (max.y <= other->Min.y) return false;

	if (min.z >= other->Max.z) return false;
	if (max.z <= other->Min.z) return false;

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

/// <summary>
/// Draws lines for each plane of the bounding box.
/// </summary>
void AABB::Debug(ColourRGBA boundsColour)
{
	
	//ColourRGBA centerColour;

	// Debug the center point.
	//renderer.DebugPoint(Center, centerColour, 1);

	Vector3 c = Center;
	Vector3 s = HalfSize;
	// Debug the bounds of the AABB, drawing the edges of the box.

	// FRONT PLANE
	// Top.
	Debug::DrawLine(Vector3(c.x - s.x, c.y + s.y, c.z + s.z), Vector3(c.x + s.x, c.y + s.y, c.z + s.z), boundsColour);

	// Bottom.
	Debug::DrawLine(Vector3(c.x - s.x, c.y - s.y, c.z + s.z), Vector3(c.x + s.x, c.y - s.y, c.z + s.z), boundsColour);

	// Left.
	Debug::DrawLine(Vector3(c.x - s.x, c.y + s.y, c.z + s.z), Vector3(c.x - s.x, c.y - s.y, c.z + s.z), boundsColour);

	// Right.
	Debug::DrawLine(Vector3(c.x + s.x, c.y + s.y, c.z + s.z), Vector3(c.x + s.x, c.y - s.y, c.z + s.z), boundsColour);

	//  DEBUG Z-axis

	// BACK PLANE
	// Top.
	Debug::DrawLine(Vector3(c.x - s.x, c.y + s.y, c.z - s.z), Vector3(c.x + s.x, c.y + s.y, c.z - s.z), boundsColour);

	// Bottom.
	Debug::DrawLine(Vector3(c.x - s.x, c.y - s.y, c.z - s.z), Vector3(c.x + s.x, c.y - s.y, c.z - s.z), boundsColour);

	// Left.
	Debug::DrawLine(Vector3(c.x - s.x, c.y + s.y, c.z - s.z), Vector3(c.x - s.x, c.y - s.y, c.z - s.z), boundsColour);

	// Right.
	Debug::DrawLine(Vector3(c.x + s.x, c.y + s.y, c.z - s.z), Vector3(c.x + s.x, c.y - s.y, c.z - s.z), boundsColour);

	// LEFT PLANE
	// top left edge front to back.
	Debug::DrawLine(Vector3(c.x - s.x, c.y + s.y, c.z + s.z), Vector3(c.x - s.x, c.y + s.y, c.z - s.z), boundsColour);

	// bottom left edge front to back.
	Debug::DrawLine(Vector3(c.x - s.x, c.y - s.y, c.z + s.z), Vector3(c.x - s.x, c.y - s.y, c.z - s.z), boundsColour);

	// RIGHT PLANE
	//top right edge front to back.
	Debug::DrawLine(Vector3(c.x + s.x, c.y + s.y, c.z + s.z), Vector3(c.x + s.x, c.y + s.y, c.z - s.z), boundsColour);

	// bottom right edge front to back.
	Debug::DrawLine(Vector3(c.x + s.x, c.y - s.y, c.z + s.z), Vector3(c.x + s.x, c.y - s.y, c.z - s.z), boundsColour);


}