#include "Vector3.h"
//namespace ath
//{

// Define and Initialise static members of class Vector3.
Vector3 Vector3::Zero       = Vector3(0, 0, 0);
Vector3 Vector3::One        = Vector3(1, 1, 1);
Vector3 Vector3::UnitX      = Vector3(1, 0, 0);
Vector3 Vector3::UnitY      = Vector3(0, 1, 0);
Vector3 Vector3::UnitZ      = Vector3(0, 0, 1);
Vector3 Vector3::Forward    = Vector3(0, 0, 1);
Vector3 Vector3::Back       = Vector3(0, 0, -1);
Vector3 Vector3::Left       = Vector3(-1, 0, 0);
Vector3 Vector3::Right      = Vector3(1, 0, 0);
Vector3 Vector3::Up         = Vector3(0, 1, 0);
Vector3 Vector3::Down       = Vector3(0, -1, 0);

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3::Vector3(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
}
Vector3::Vector3(Vector4 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}
Vector3::Vector3(Vector2 v)
{
	x = v.x;
	y = v.y;
	z = 0;
}

Vector3 Vector3::Normalised()
{
	return Normalise();
}

float Vector3::MagnitudeSquared()
{
	return sqrt(Dot(*this, *this));
}

float Vector3::Magnitude()
{
	return Dot(*this, *this);
}

Vector2 Vector3::xz()
{
	Vector2 result(x, z);
	return result;
}
void Vector3::xz(Vector2 xz)
{
	x = xz.x;
	z = xz.y;
}

Vector2 Vector3::yz()
{
	Vector2 result(y, z);
	return result;
}
void Vector3::yz(Vector2 yz)
{
	x = yz.x;
	y = yz.y;
}
void Vector3::xy(Vector2 xy)
{
	x = xy.x;
	y = xy.y;
}
Vector2 Vector3::xy()
{
	Vector2 result(x, y);
	return result;
}

void Vector3::xyi(Vector2i xyi)
{
	x = xyi.x;
	y = xyi.y;
}
Vector2i Vector3::xyi()
{
	Vector2i result((int)x, (int)y);
	return result;
}

Vector4 Vector3::xyzw()
{
	Vector4 result(x, y, z, 0.0);
	return result;
}

void Vector3::xyzw(Vector4 xyzw)
{
	x = xyzw.x;
	y = xyzw.y;
	z = xyzw.z;
}


//Vector3 Vector3::TransformVector(Matrix4 transformationMatrix)
//{
//	Vector4 v(*this, 1.0f);

//	return (transformationMatrix * v).xyz();
//}

//Vector3 Vector3::TransformNormal(Matrix4 transformationMatrix)
//{
//	Vector3 result;
//	Matrix4 m = transformationMatrix.Inverse();

//	Vector3 r0(m.GetRow0());
//	Vector3 r1(m.GetRow1());
//	Vector3 r2(m.GetRow2());

//	Vector3 tmp
//	(
//		Dot(*this, r0),
//		Dot(*this, r1),
//		Dot(*this, r2)
//	);

//	result = tmp;

//	return result;
//}

//Vector3 Vector3::TransformVector(Matrix4 transformationMatrix, bool ignoreTraslation = false, bool isNormal = false)
//{
//	Vector3 result;
//	Matrix4 m = transformationMatrix;
//
//	if (isNormal)
//	{
//		m = transformationMatrix.Inverse();
//
//		Vector3 r0(m.GetRow0());
//		Vector3 r1(m.GetRow1());
//		Vector3 r2(m.GetRow2());
//
//		Vector3 tmp
//		(
//			Dot(*this, r0),
//			Dot(*this, r1),
//			Dot(*this, r2)
//		);
//
//		result = tmp;
//	}
//	else
//	{
//		Vector3 c0(m.GetColumn0());
//		Vector3 c1(m.GetColumn0());
//		Vector3 c2(m.GetColumn0());
//
//		Vector3 tmp
//		(
//			Dot(*this, c0) + (ignoreTraslation ? 0 : m.GetRow3().x),
//			Dot(*this, c1) + (ignoreTraslation ? 0 : m.GetRow3().y),
//			Dot(*this, c2) + (ignoreTraslation ? 0 : m.GetRow3().z)
//		);
//
//		return tmp;
//	}
//
//	return result;
//}

Vector3 Vector3::Add(Vector3 left, Vector3 right)
{
	Vector3 result(left.x + right.x, left.y + right.y, left.z + right.z);
	return result;
}

Vector3 Vector3::Add(Vector3 left, float scalar)
{
	Vector3 result(left.x + scalar, left.y + scalar, left.z + scalar);
	return result;
}

Vector3 Vector3::Subtract(Vector3 left, Vector3 right)
{
	Vector3 result(left.x - right.x, left.y - right.y, left.z - right.z);
	return result;
}

Vector3 Vector3::Subtract(float scalar, Vector3 right)
{
	Vector3 result(scalar - right.x, scalar - right.y, scalar - right.z);
	return result;
}

Vector3 Vector3::Subtract(Vector3 left, float scalar)
{
	Vector3 result(left.x - scalar, left.y - scalar, left.z - scalar);
	return result;
}

Vector3 Vector3::Multiply(Vector3 left, Vector3 right)
{
	Vector3 result(left.x * right.x, left.y * right.y, left.z * right.z);
	return result;
}

Vector3 Vector3::Multiply(Vector3 left, float right)
{
	Vector3 result(left.x * right, left.y * right, left.z * right);
	return result;
}

Vector3 Vector3::Scale(Vector3 left, float scalar)
{
	Vector3 result(left.x * scalar, left.y * scalar, left.z * scalar);
	return result;
}
Vector3 Vector3::Divide(Vector3 left, Vector3 right)
{
	Vector3 result(left.x / right.x, left.y / right.y, left.z / right.z);
	return result;
}

Vector3 Vector3::Divide(Vector3 left, float scalar)
{
	Vector3 result(left.x / scalar, left.y / scalar, left.z / scalar);
	return result;
}

Vector3 Vector3::Add(Vector3 v)
{
	return Add(*this, v);
}

Vector3 Vector3::Multiply(Vector3 v)
{
	return Multiply(*this, v);
}

Vector3 Vector3::Translate(Vector3 translation)
{
	Vector3 result(x + translation.x, y + translation.y, z + translation.z);
	return result;
}

Vector3 Vector3::Scale(Vector3 s)
{
	Vector3 result(x * s.x, y * s.y, z * s.z);
	return result;
}
Vector3 Vector3::Scale(float s)
{
	return Scale(*this, s);
}

Vector3 Vector3::RotateXY(float xAngleDegrees, float yAngleDegrees)
{
	Vector3 result(
		(x * cos(xAngleDegrees * Deg2Rad)) - (x * sin(xAngleDegrees * Deg2Rad)),
		(y * sin(yAngleDegrees * Deg2Rad)) + (y * cos(yAngleDegrees * Deg2Rad)),
		z
	);

	return result;
}

Vector3 Vector3::Normalise()
{
	float magnitude = MagnitudeSquared();

	if(magnitude == 0)
	{
		return Vector3::Zero;
	}

	Vector3 result(x / magnitude, y / magnitude, z / magnitude);

	return result;
}

float Vector3::GetMagnitudeSquared()
{
	return sqrt(Dot(*this, *this));
}

float Vector3::GetMagnitude()
{
	return Dot(*this, *this);
}

float Vector3::Distance(Vector3 source, Vector3 target)
{
	return (source.x * target.x + source.y * target.y + source.z * target.z);
}

float Vector3::DistanceSquared(Vector3 source, Vector3 target)
{
	return sqrt(source.x * target.x + source.y * target.y + source.z * target.z);
}

Vector3 Vector3::Reflect(Vector3 vertexPosition, Vector3 normal)
{
	return (normal - vertexPosition) * (2.0 * Dot(vertexPosition, normal));
}

float Vector3::Dot(Vector3 left, Vector3 right)
{
	return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

Vector3 Vector3::Cross(Vector3 left, Vector3 right)
{
	Vector3 result(
		(left.y * right.z) - (left.z * right.y),
		(left.z * right.x) - (left.x * right.z),
		(left.x * right.y) - (left.y * right.x)
	);
	return result;
}

float Vector3::Crossf(Vector3 left, Vector3 right)
{

	return (left.y * right.z) - (left.z * right.y) -
		(left.z * right.x) - (left.x * right.z) -
		(left.x * right.y) - (left.y * right.x);

	//return (left.x * right.y) - (right.x * left.y);
}

Vector3 Vector3::Lerp(Vector3 from, Vector3 to, float ratio)
{
	return from + ((to - from) * ratio);
}

Vector3 Vector3::TriangleLerp(Vector3 a, Vector3 b, Vector3 c, float u, float v)
{
	float w = (1 - u - v);

	return (a + ((b - a) * u) + ((c - a) * v));
}

Vector3 Vector3::BarycetricInterpolator(Vector3 a, Vector3 b, Vector3 c, float ratioX, float ratioY)
{
	return TriangleLerp(a, b, c, ratioX, ratioY); // u v
}

float* Vector3::ToArray()
{
	float* result = new float[3]
	{
		x,y,z
	};
	return result;
}
//}
