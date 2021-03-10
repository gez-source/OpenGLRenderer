#ifndef VECTOR4_H
#define VECTOR4_H

#include "MathExt.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Vector2i.h"
#include "ColourRGBA.h"

//namespace ath
//{
class Vector4;
class Vector3;
class Vector2;
class Vector2i;
class ColourRGBA;
//class Matrix4;

class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;

	static Vector4 Zero;
	static Vector4 One;
	static Vector4 UnitX;
	static Vector4 UnitY;
	static Vector4 UnitZ;
	static Vector4 UnitW;

	Vector4();
	Vector4(Vector3 v3);
	Vector4(Vector3 v3, float W);
	Vector4(float X, float Y, float Z, float W);

	Vector2 xy();
	void xy(Vector2 xy);
	Vector2i xyi();
	void xyi(Vector2i xyi);
	Vector3 xyz();
	void xyz(Vector3 xyz);
	Vector3 xyw();
	void xyw(Vector3 xyw);
	Vector4 xyzw();
	void xyzw(Vector4 xyzw);

	Vector4 Translate(Vector4 translation);

	Vector4 Scale(Vector4 s);

	Vector4 Scale(float s);

	Vector4 RotateXY(float xAngleDegrees, float yAngleDegrees);

	Vector4 Normalised();

	float MagnitudeSquared();

	float Magnitude();

	Vector4 Normalise();

	float GetMagnitudeSquared();

	float GetMagnitude();

	float Combine();

	//const Vector4& TransformVector(const Matrix4& transformationMatrix);

	//static const Vector4& TransformVector(const Vector4& vector, const Matrix4& matrix);

	static float Dot(Vector4 left, Vector4 right);

	static Vector4 Reflect(Vector4 vertexPosition, Vector4 normal);

	static Vector4 Lerp(Vector4 from, Vector4 to, float ratio);

	static Vector4 TriangleLerp(Vector4 a, Vector4 b, Vector4 c, float ratioX, float ratioY);

	static Vector4 BarycetricInterpolator(Vector4 a, Vector4 b, Vector4 c, float ratioX, float ratioY);

	static Vector4 Add(Vector4 left, Vector4 right);

	static Vector4 Add(Vector4 left, float scalar);

	static Vector4 Subtract(Vector4 left, Vector4 right);

	static Vector4 Subtract(Vector4 left, float scalar);

	static Vector4 Subtract(float scalar, Vector4 right);

	static Vector4 Multiply(Vector4 left, Vector4 right);

	static Vector4 Multiply(Vector4 left, float right);

	static Vector4 Scale(Vector4 left, float scalar);

	static Vector4 Divide(Vector4 left, Vector4 right);

	static Vector4 Divide(Vector4 left, float scalar);

	// Operators.

	/// Get a vector component given the specified index.
	float operator [] (int i)
	{
		return (&x)[i];
	}
	//operator Vector4()
	//{
	//	Vector4 result(x, y, z, w);
	//	return result;
	//}
	//operator ColourRGBA()
    //{
    //    ColourRGBA result(w, x, y, z);
    //    return result;
    //}
};

// Operators

inline Vector4 operator -(Vector4 left)
{
	Vector4 result(-left.x, -left.y, -left.z, -left.w);
	return result;
}

inline bool operator ==(Vector4 left, Vector4 right)
{
	// Provide a tolerance for floating point error.
	return (fabs(left.x - right.x) <= FLT_EPSILON)
		&& (fabs(left.y - right.y) <= FLT_EPSILON)
		&& (fabs(left.z - right.z) <= FLT_EPSILON)
		&& (fabs(left.w - right.w) <= FLT_EPSILON);
	//return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
}
inline bool operator !=(Vector4 left, Vector4 right)
{
	// Provide a tolerance for floating point error.
	return !((fabs(left.x - right.x) <= FLT_EPSILON)
		&& (fabs(left.y - right.y) <= FLT_EPSILON)
		&& (fabs(left.z - right.z) <= FLT_EPSILON)
		&& (fabs(left.w - right.w) <= FLT_EPSILON));
	//return (left.x != right.x) || (left.y != right.y) || (left.z != right.z) || (left.w == right.w);
}

inline bool operator < (Vector4 left, Vector4 right)
{
	return (left.x < right.x) || (left.y < right.y) || (left.z < right.z) || (left.w < right.w);
}
inline bool operator > (Vector4 left, Vector4 right)
{
	return (left.x > right.x) || (left.y > right.y) || (left.z > right.z) || (left.w > right.w);
}

//Vector4 operator +(Vector4 right){ return Add(*this, right); }
//Vector4 operator +(int right) { return Add(*this, right); }
//Vector4 operator -(Vector4 right) { return Subtract(*this, right); }
//Vector4 operator -(float right) { return Subtract(*this, right); }
//Vector4 operator *(float right) { return Multiply(*this, right); }
//Vector4 operator *(Vector4 right) { return Multiply(*this, right); }
//Vector4 operator /(float right) { return Divide(*this, right); }
//Vector4 operator /(Vector4 right) { return Divide(*this, right); }

inline Vector4 operator +(Vector4 left, Vector4 right) { return Vector4::Add(left, right); }
inline Vector4 operator +(Vector4 left, float right) { return Vector4::Add(left, right); }
inline Vector4 operator -(Vector4 left, Vector4 right) { return Vector4::Subtract(left, right); }
inline Vector4 operator -(Vector4 left, float right) { return Vector4::Subtract(left, right); }
inline Vector4 operator -(float left, Vector4 right) { return Vector4::Subtract(left, right); }
inline Vector4 operator *(Vector4 left, Vector4 right) { return Vector4::Multiply(left, right); }
inline Vector4 operator *(Vector4 left, float right) { return Vector4::Scale(left, right); }
inline Vector4 operator /( Vector4 left, Vector4 right) { return Vector4::Divide(left, right); }
inline Vector4 operator /(Vector4 left, float right) { return Vector4::Divide(left, right); }

inline Vector4 operator +=(Vector4 left, Vector4 right)
{
    Vector4 result = Vector4(left.x, left.y, left.z, left.w);
	result.x += right.x;
	result.y += right.y;
	result.z += right.z;
	result.w += right.w;
	return result;
}

inline Vector4 operator *=(Vector4 left, Vector4 right)
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;
	left.w *= right.w;
	return left;
}

inline Vector4 operator *=(Vector4 left, float right)
{
	left.x *= right;
	left.y *= right;
	left.z *= right;
	left.w *= right;
	return left;
}

inline Vector4 operator /=(Vector4 left, Vector4 right)
{
	left.x /= right.x;
	left.y /= right.y;
	left.z /= right.z;
	left.w /= right.w;
	return left;
}

inline Vector4 operator -=(Vector4 left, Vector4 right)
{
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;
	left.w -= right.w;
	return left;
}

//operator Vector3()
//{
//	Vector3 result(x, y, 0);
//	return result;
//}

//operator Vector2()
//{
//	Vector2 result(x, y);
//	return result;
//}

//operator Vector2i()
//{
//	Vector2i result((int)x, (int)y);
//	return result;
//}

//}
#endif
