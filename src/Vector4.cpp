#include "Vector4.h"

//namespace ath
//{

// Define and Initialise static members of class Vector4.
Vector4 Vector4::Zero  = Vector4(0, 0, 0, 0);
Vector4 Vector4::One   = Vector4(1, 1, 1, 1);
Vector4 Vector4::UnitX = Vector4(1, 0, 0, 0);
Vector4 Vector4::UnitY = Vector4(0, 1, 0, 0);
Vector4 Vector4::UnitZ = Vector4(0, 0, 1, 0);
Vector4 Vector4::UnitW = Vector4(0, 0, 0, 1);

Vector4::Vector4()
{
	x = 0; y = 0; z = 0; w = 0;
}

Vector4::Vector4(Vector3 v3)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = 0.0f;
}

Vector4::Vector4(Vector3 v3, float W)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = W;
}

Vector4::Vector4(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

Vector2 Vector4::xy()
{
	Vector2 result(x, y);
	return result;
}
void Vector4::xy(Vector2 xy)
{
	x = xy.x;
	y = xy.y;
}

Vector2i Vector4::xyi()
{
	Vector2i result((int)x, (int)y);
	return result;
}
void Vector4::xyi(Vector2i xyi)
{
	x = xyi.x;
	y = xyi.y;
}

Vector3 Vector4::xyz()
{
	Vector3 result(x, y, z);
	return result;
}
void Vector4::xyz(Vector3 xyz)
{
	x = xyz.x;
	y = xyz.y;
	z = xyz.z;
}

Vector3 Vector4::xyw()
{
	Vector3 result(x, y, w);
	return result;
}

void Vector4::xyw(Vector3 xyw)
{
	x = xyw.x;
	y = xyw.y;
	w = xyw.z;
}

Vector4 Vector4::xyzw()
{
	Vector4 result(x, y, z, w);
	return result;
}

void Vector4::xyzw(Vector4 xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
	w = xyzw.w;
}

Vector4 Vector4::Translate(Vector4 translation)
{
	Vector4 result(x + translation.x, y + translation.y, z + translation.z, w + translation.w);
	return result;
}

Vector4 Vector4::Scale(Vector4 s)
{
	Vector4 result(x * s.x, y * s.y, z * s.z, w * s.w);
	return result;
}

Vector4 Vector4::Scale(float s)
{
	return Scale(*this, s);
}

Vector4 Vector4::RotateXY(float xAngleDegrees, float yAngleDegrees)
{
	Vector4 result(
		(x * cos(xAngleDegrees * Deg2Rad)) - (x * std::sin(xAngleDegrees * Deg2Rad)),
		(y * sin(yAngleDegrees * Deg2Rad)) + (y * std::cos(yAngleDegrees * Deg2Rad)),
		z,
		w
	);
	return result;
}

Vector4 Vector4::Normalised()
{
	return Normalise();
}

float Vector4::MagnitudeSquared()
{
	return std::sqrt(Dot(*this, *this));
}

float Vector4::Magnitude()
{
	return Dot(*this, *this);
}

Vector4 Vector4::Normalise()
{
	float magnitude = MagnitudeSquared();
	Vector4 n(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
	return n;
}

float Vector4::GetMagnitudeSquared()
{
	return sqrt(Dot(*this, *this));
}

float Vector4::GetMagnitude()
{
	return Dot(*this, *this);
}

float Vector4::Combine()
{
	return x * y * z * w;
}

//Vector4 Vector4::TransformVector(Matrix4 transformationMatrix)
//{
//	Vector4 vector = *this;
//	Vector4 result = (transformationMatrix * vector);
//}

//Vector4 Vector4::TransformVector(Vector4 vector, Matrix4 matrix)
//{
//	Vector4 result(
//		Dot(vector, matrix.GetColumn0()),
//		Dot(vector, matrix.GetColumn1()),
//		Dot(vector, matrix.GetColumn2()),
//		Dot(vector, matrix.GetColumn3())
//	);
//	return result;
//}

float Vector4::Dot(Vector4 left, Vector4 right)
{
	return (left.x * right.x)
		+ (left.y * right.y)
		+ (left.z * right.z)
		+ (left.w * right.w);
}

Vector4 Vector4::Reflect(Vector4 vertexPosition, Vector4 normal)
{
	return (normal - vertexPosition) * (2.0 * Dot(vertexPosition, normal));
}

Vector4 Vector4::Lerp(Vector4 from, Vector4 to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector4 Vector4::TriangleLerp(Vector4 a, Vector4 b, Vector4 c, float ratioX, float ratioY)
{
	return a + ((b - a) * ratioX) + ((c - a) * ratioY);
}

Vector4 Vector4::BarycetricInterpolator(Vector4 a, Vector4 b, Vector4 c, float ratioX, float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY);
}

Vector4 Vector4::Add(Vector4 left, Vector4 right)
{
	Vector4 result(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	return result;
}

Vector4 Vector4::Add(Vector4 left, float scalar)
{
	Vector4 result(left.x + scalar, left.y + scalar, left.z + scalar, left.w + scalar);
	return result;
}

Vector4 Vector4::Subtract(Vector4 left, Vector4 right)
{
	Vector4 result(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	return result;
}

Vector4 Vector4::Subtract(Vector4 left, float scalar)
{
	Vector4 result(left.x - scalar, left.y - scalar, left.z - scalar, left.w - scalar);
	return result;
}

Vector4 Vector4::Subtract(float scalar, Vector4 right)
{
	Vector4 result(scalar - right.x, scalar - right.y, scalar - right.z, scalar - right.w);
	return result;
}

Vector4 Vector4::Multiply(Vector4 left, Vector4 right)
{
	Vector4 result(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	return result;
}

Vector4 Vector4::Multiply(Vector4 left, float right)
{
	Vector4 result(left.x * right, left.y * right, left.z * right, left.w * right);
	return result;
}

Vector4 Vector4::Scale(Vector4 left, float scalar)
{
	Vector4 result(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
	return result;
}
Vector4 Vector4::Divide(Vector4 left, Vector4 right)
{
	Vector4 result(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	return result;
}

Vector4 Vector4::Divide(Vector4 left, float scalar)
{
	Vector4 result(left.x / scalar, left.y / scalar, left.z / scalar, left.w / scalar);
	return result;
}
//}
