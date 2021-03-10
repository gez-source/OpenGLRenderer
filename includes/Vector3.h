#ifndef VECTOR3_H
#define VECTOR3_H

#include "MathExt.h"
#include "Vector4.h"
#include "Vector2i.h"
#include "Vector2.h"
#include "Matrix4.h"
//#include <SFML/System/Vector3.hpp>

//namespace ath
//{

class Vector4;
class Vector3;
class Vector2;
class Vector2i;

class Vector3
{
public:
	float x;
	float y;
	float z;

	static Vector3 Zero;
	static Vector3 One;
	static Vector3 Forward;
	static Vector3 Back;
	static Vector3 Left;
	static Vector3 Right;
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 UnitX;
	static Vector3 UnitY;
	static Vector3 UnitZ;

	Vector3();
	Vector3(float X, float Y, float Z);
	Vector3(Vector4 v);
	Vector3(Vector2 v);

	Vector3 Normalised();
	float MagnitudeSquared();
	float Magnitude();

	Vector2 xz();
	void xz(Vector2 xz);
	Vector2 yz();
	void yz(Vector2 xy);
	Vector2 xy();
	void xy(Vector2 xy);
    Vector2i xyi();
	void xyi(Vector2i xyi);
	Vector4 xyzw();
	void xyzw(Vector4 xyzw);

	//Vector3 TransformVector(Matrix4 transformationMatrix);
	//Vector3 TransformNormal(Matrix4 transformationMatrix);
	Vector3 Add(Vector3 v);
	Vector3 Multiply(Vector3 v);
	Vector3 Translate(Vector3 translation);
	Vector3 Scale(Vector3 s);
	Vector3 Scale(float s);
	Vector3 RotateXY(float xAngleDegrees, float yAngleDegrees);
	Vector3 Normalise();
	float GetMagnitudeSquared();
	float GetMagnitude();


	static float Distance(Vector3 source, Vector3 target);
	static float DistanceSquared(Vector3 source, Vector3 target);
	static Vector3 Reflect(Vector3 vertexPosition, Vector3 normal);
	static float Dot(Vector3 left, Vector3 right);
	static Vector3 Cross(Vector3 left, Vector3 right);
	static float Crossf(Vector3 left, Vector3 right);
	static Vector3 Lerp(Vector3 from, Vector3 to, float ratio);
	static Vector3 TriangleLerp(Vector3 a, Vector3 b, Vector3 c, float u, float v);
	static Vector3 BarycetricInterpolator(Vector3 a, Vector3 b, Vector3 c, float ratioX, float ratioY);
	static Vector3 Add(Vector3 left, Vector3 right);
	static Vector3 Add(Vector3 left, float scalar);
	static Vector3 Subtract(Vector3 left, Vector3 right);
	static Vector3 Subtract(float scalar, Vector3 right);
	static Vector3 Subtract(Vector3 left, float scalar);
	static Vector3 Multiply(Vector3 left, Vector3 right);
	static Vector3 Multiply(Vector3 left, float right);
	static Vector3 Scale(Vector3 left, float scalar);
	static Vector3 Divide(Vector3 left, Vector3 right);
	static Vector3 Divide(Vector3 left, float scalar);

	float* ToArray();

	//Vector3 operator +(Vector3 right) { return Add(*this, right); }
	//Vector3 operator +(int right){ return Add(*this, right); }
	//Vector3 operator -(Vector3 right){ return Subtract(*this, right); }
	//Vector3 operator -(float right){ return Subtract(*this, right); }
	//Vector3 operator *(float right){ return Multiply(*this, right); }
	//Vector3 operator *(Vector3 right){ return Multiply(*this, right); }
	//Vector3 operator /(float right){ return Divide(*this, right); }
	//Vector3 operator /(Vector3 right){ return Divide(*this, right); }

	//Vector3 operator +(Vector3 left, Vector3 right) { return Add(left, right); }
	//Vector3 operator +(Vector3 left, float right) { return Add(left, right); }
	//Vector3 operator -(Vector3 left, Vector3 right){ return Subtract(left, right); }
	//Vector3 operator -(Vector3 left, float right) { return Subtract(left, right); }
	//Vector3 operator -(float left, Vector3 right){ return Subtract(left, right); }
	//Vector3 operator *(Vector3 left, Vector3 right){ return Multiply(left, right); }
	//Vector3 operator *(Vector3 left, float right){ return Scale(left, right); }
	//Vector3 operator /(Vector3 left, Vector3 right){ return Divide(left, right); }
	//Vector3 operator /(Vector3 left, float right){ return Divide(left, right); }

	//operator Vector4()
	//{
	//	Vector4 result(x, y, z, 0.0);
	//	return result;
	//}

	//operator Vector2()
	//{
	//	Vector2 result(x, y);
	//	return result;
	//}

	//operator Vector2i()
	//{
	//	int _x = x;
	//	int _y = y;
	//	Vector2i result(_x, _y);
	//	return result;
	//}
	/// Get a vector component given the specified index.
	float operator [] (int i)
	{
		return (&x)[i];
	}

	//operator sf::Vector3f()
	//{
	//	sf::Vector3f result(x, y, z);
	//	return result;
	//}
};

// Operators

inline Vector3 operator +(Vector3 left, Vector3 right) { return Vector3::Add(left, right); }
inline Vector3 operator +(Vector3 left, float right) { return Vector3::Add(left, right); }
inline Vector3 operator -(Vector3 left, Vector3 right) { return Vector3::Subtract(left, right); }
inline Vector3 operator -(Vector3 left, float right) { return Vector3::Subtract(left, right); }
inline Vector3 operator -(float left, Vector3 right) { return Vector3::Subtract(left, right); }
inline Vector3 operator *(Vector3 left, Vector3 right) { return Vector3::Multiply(left, right); }
inline Vector3 operator *(Vector3 left, float right) { return Vector3::Scale(left, right); }
inline Vector3 operator /( Vector3 left, Vector3 right) { return Vector3::Divide(left, right); }
inline Vector3 operator /(Vector3 left, float right) { return Vector3::Divide(left, right); }

//inline Vector3 operator -(Vector3 left)
//{
//	return Vector3(-left.x, -left.y, -left.z);
//}

inline bool operator ==(Vector3 left, Vector3 right)
{
	return (fabs(left.x - right.x) <= FLT_EPSILON)
		&& (fabs(left.y - right.y) <= FLT_EPSILON)
		&& (fabs(left.z - right.z) <= FLT_EPSILON);
	//return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

inline bool operator !=(Vector3 left, Vector3 right)
{
	return !((fabs(left.x - right.x) <= FLT_EPSILON)
		&& (fabs(left.y - right.y) <= FLT_EPSILON)
		&& (fabs(left.z - right.z) <= FLT_EPSILON));
	//return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}

//inline Vector3 operator +=(Vector3 left, Vector3 right)
//{
//	left.x += right.x;
//	left.y += right.y;
//	left.z += right.z;
//	return left;
//}

//inline Vector3 operator *=(Vector3 left, Vector3 right)
//{
//	left.x *= right.x;
//	left.y *= right.y;
//	left.z *= right.z;
//	return left;
//}

//inline Vector3 operator -=(Vector3 left, Vector3 right)
//{
//	left.x -= right.x;
//	left.y -= right.y;
//	left.z -= right.z;
//	return left;
//}

//inline Vector3 operator /=(Vector3 left, Vector3 right)
//{
//	left.x /= right.x;
//	left.y /= right.y;
//	left.z /= right.z;
//	return left;
//}

inline bool operator < (Vector3 left, Vector3 right)
{
	return (left.x < right.x) || (left.y < right.y) || (left.z < right.z);
}

inline bool operator > (Vector3 left, Vector3 right)
{
	return (left.x > right.x) || (left.y > right.y) || (left.z > right.z);
}
//}

#endif
