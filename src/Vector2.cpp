#include "Vector2.h"
//namespace ath
//{
// Define and Initialise static members of class Vector2.
Vector2 Vector2::Zero       = Vector2(0, 0);
Vector2 Vector2::One        = Vector2(1, 1);
Vector2 Vector2::UnitX      = Vector2(1, 0);
Vector2 Vector2::UnitY      = Vector2(0, 1);

Vector2::Vector2()
{
	x = 0; y = 0;
}

Vector2::Vector2(float X, float Y)
{
	x = X;
	y = Y;
}

Vector2i Vector2::xyi()
{
	Vector2i result(x, y);
	return result;
}

void Vector2::xyi(Vector2i xyi)
{
	x = xyi.x;
	y = xyi.y;
}

Vector2 Vector2::xy()
{
	Vector2 result(x, y);
	return result;
}

void Vector2::xy(Vector2 xy)
{
	x = xy.x;
	y = xy.y;
}

Vector3 Vector2::xyz()
{
	Vector3 result(x, y, 0.0);
	return result;
}

void Vector2::xyz(Vector3 xyz)
{
	x = xyz.x;
	y = xyz.y;
}

Vector4 Vector2::xyzw()
{
	Vector4 result(x, y, 0.0, 0.0);
	return result;
}

void Vector2::xyzw(Vector4 xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
}

Vector2 Vector2::Normalised()
{
	return Normalise();
}

Vector2 Vector2::Translate(Vector2 translation)
{
	Vector2 result(x + translation.x, y + translation.y);
	return result;
}

Vector2 Vector2::Scale(Vector2 s)
{
	Vector2 result(x * s.x, y * s.y);
	return result;
}

Vector2 Vector2::Scale(float s)
{
	return Scale(*this, s);
}

Vector2 Vector2::RotateXY(float xAngleDegrees, float yAngleDegrees)
{
	Vector2 result(
		(x * std::cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)),
		(y * std::sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad))
	);

	return result;
}

float Vector2::MagnitudeSquared()
{
	return std::sqrt(Dot(*this, *this));
}

float Vector2::Magnitude()
{
	return Dot(*this, *this);
}

float Vector2::GetMagnitudeSquared()
{
	return std::sqrt(Dot(*this, *this));
}

float Vector2::GetMagnitude()
{
	return Dot(*this, *this);
}

Vector2 Vector2::Normalise()
{
	float magnitude = MagnitudeSquared();
	Vector2 result(x / magnitude, y / magnitude);
	return result;
}


float Vector2::Dot(Vector2 left, Vector2 right)
{
	return (left.x * right.x) + (left.y * right.y);
}

float Vector2::Cross(Vector2 left, Vector2 right)
{
	return (left.x * right.y) - (right.x * left.y);
}

float Vector2::Lerp(float from, float to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector2 Vector2::Lerp(Vector2 from, Vector2 to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector2 Vector2::TriangleLerp(Vector2 a, Vector2 b, Vector2 c, float ratioX, float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

Vector2 Vector2::BarycetricInterpolator(Vector2 a, Vector2 b, Vector2 c, float ratioX, float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

Vector2 Vector2::Add(Vector2 left, Vector2 right)
{
	Vector2 result(left.x + right.x, left.y + right.y);
	return result;
}

Vector2 Vector2::Add(Vector2 left, float scalar)
{
	Vector2 result(left.x + scalar, left.y + scalar);
	return result;
}

Vector2 Vector2::Subtract(Vector2 left, Vector2 right)
{
	Vector2 result(left.x - right.x, left.y - right.y);
	return result;
}

Vector2 Vector2::Subtract(Vector2 left, float scalar)
{
	Vector2 result(left.x - scalar, left.y - scalar);
	return result;
}

Vector2 Vector2::Multiply(Vector2 left, Vector2 right)
{
	Vector2 result(left.x * right.x, left.y * right.y);
	return result;
}

Vector2 Vector2::Multiply(Vector2 left, float right)
{
	Vector2 result(left.x * right, left.y * right);
	return result;
}

Vector2 Vector2::Scale(Vector2 left, float scalar)
{
	Vector2 result(left.x * scalar, left.y * scalar);
	return result;
}
Vector2 Vector2::Divide(Vector2 left, Vector2 right)
{
	Vector2 result(left.x / right.x, left.y / right.y);
	return result;
}

Vector2 Vector2::Divide(Vector2 left, float scalar)
{
	Vector2 result(left.x / scalar, left.y / scalar);
	return result;
}

//}
