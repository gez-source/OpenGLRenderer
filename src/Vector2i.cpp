#include "Vector2i.h"

//namespace ath
//{
// Define and Initialise static members of class Vector2i.
Vector2i Vector2i::Zero         = Vector2i(0, 0);
Vector2i Vector2i::One          = Vector2i(1, 1);
Vector2i Vector2i::UnitX        = Vector2i(1, 0);
Vector2i Vector2i::UnitY        = Vector2i(0, 1);

Vector2i::Vector2i()
{
	x = 0; y = 0;
}

Vector2i::Vector2i(int X, int Y)
{
	x = X;
	y = Y;
}

Vector2i::Vector2i(float X, float Y)
{
	x = static_cast<int>(X);
	y = static_cast<int>(Y);
}

Vector2 Vector2i::xy() 
{
	return Vector2(x, y);
}

void Vector2i::xy(Vector2 xy)
{
	x = static_cast<int>(xy.x);
	y = static_cast<int>(xy.y);
}

Vector3 Vector2i::xyz() 
{
	return Vector3(x, y, 0);
}

void Vector2i::xyz(Vector3 xyz)
{
	x = static_cast<int>(xyz.x);
	y = static_cast<int>(xyz.y);
}

Vector4 Vector2i::xyzw() 
{
	return Vector4(x, y, 0, 0);
}

Vector3 Vector2i::xyw()
{
	Vector3 result(x, y, 0);
	return result;
}

void Vector2i::xyw(Vector3 xyw)
{
	x = static_cast<int>(xyw.x);
	y = static_cast<int>(xyw.y);
}
Vector2i Vector2i::Translate(Vector2i translation) 
{
	Vector2i result((x + translation.x), (y + translation.y));
	return result;
}

Vector2i Vector2i::Scale(Vector2i s) 
{
	Vector2i result(x * s.x, y * s.y);
	return result;
}

Vector2i Vector2i::Scale(float s) 
{
	return Scale(*this, s);
}

Vector2i Vector2i::RotateXY(float xAngleDegrees, float yAngleDegrees) 
{
	Vector2i result(
		static_cast<int>(((x * std::cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)))),
		static_cast<int>(((y * std::sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad))))
	);
	return result;
}

float Vector2i::MagnitudeSquared() 
{
	return std::sqrt(Dot(*this, *this));
}

float Vector2i::Magnitude() 
{
	return Dot(*this, *this);
}

float Vector2i::GetMagnitudeSquared() 
{
	return std::sqrt(Dot(*this, *this));
}

float Vector2i::GetMagnitude() 
{
	return (x * x) + (y * y);
}

// Static functions.
float Vector2i::Dot(Vector2i left, Vector2i right)
{
	return (left.x * right.x) + (left.y * right.y);
}

float Vector2i::Cross(Vector2i left, Vector2i right)
{
	return (left.x * right.y) - (right.x * left.y);
}

float Vector2i::Lerp(float from, float to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector2i Vector2i::Lerp(Vector2i from, Vector2i to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector2i Vector2i::TriangleLerp(Vector2i a, Vector2i b, Vector2i c, float ratioX, float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

Vector2i Vector2i::BarycetricInterpolator(Vector2i a, Vector2i b, Vector2i c, float ratioX, float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

Vector2i Vector2i::Add(Vector2i left, Vector2i right)
{
	Vector2i result(left.x + right.x, left.y + right.y);
	return result;
}

Vector2i Vector2i::Add(Vector2i left, int scalar)
{
	Vector2i result(left.x + scalar, left.y + scalar);
	return result;
}

Vector2i Vector2i::Subtract(Vector2i left, Vector2i right)
{
	Vector2i result(left.x - right.x, left.y - right.y);
	return result;
}

Vector2i Vector2i::Subtract(Vector2i left, int scalar)
{
	Vector2i result(left.x - scalar, left.y - scalar);
	return result;
}

Vector2i Vector2i::Subtract(float scalar, Vector2i right)
{
	Vector2i result(scalar - right.x, scalar - right.y);
	return result;
}

Vector2i Vector2i::Multiply(Vector2i left, Vector2i right)
{
	Vector2i result(left.x * right.x, left.y * right.y);
	return result;
}

Vector2i Vector2i::Multiply(Vector2i left, float right)
{
	Vector2i result(left.x * right, left.y * right);
	return result;
}

Vector2i Vector2i::Scale(Vector2i left, float scalar)
{
	Vector2i result(static_cast<int>(left.x * scalar), static_cast<int>(left.y * scalar));
	return result;
}

Vector2i Vector2i::Scale(Vector2i left, int scalar)
{
	Vector2i result(left.x * scalar, left.y * scalar);
	return result;
}

Vector2i Vector2i::Divide(Vector2i left, Vector2i right)
{
	Vector2i result(left.x / right.x, left.y / right.y);
	return result;
}

Vector2i Vector2i::Divide(Vector2i left, int scalar)
{
	Vector2i result(left.x / scalar, left.y / scalar);
	return result;
}

//operator Vector2i::Vector2()
//{
//	Vector2 result((int)x, (int)y);
//	return result;
//}
//}
