#ifndef VECTOR2_H
#define VECTOR2_H
//namespace ath
//{
#include <algorithm>
#include "MathExt.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2i.h"

class Vector4;
class Vector3;
class Vector2;
class Vector2i;

class Vector2
{
public:
	float x;
	float y;

	static Vector2 Zero;
	static Vector2 One;
	static Vector2 UnitX;
	static Vector2 UnitY;

	Vector2();
	Vector2( float X,  float Y);

	 Vector2i xyi() ;
	void xyi( Vector2i xyi);
	 Vector2 xy() ;
	void xy( Vector2 xy);
	 Vector3 xyz() ;
	void xyz( Vector3 xyz);
	 Vector4 xyzw() ;
	void xyzw( Vector4 xyzw);

	 Vector2 Normalised() ;

	 Vector2 Translate( Vector2 translation) ;

	 Vector2 Scale( Vector2 s) ;

	 Vector2 Scale( float s) ;

	 Vector2 RotateXY( float xAngleDegrees,  float yAngleDegrees) ;

	 float MagnitudeSquared() ;

	 float Magnitude() ;

	 float GetMagnitudeSquared() ;

	 float GetMagnitude() ;

	 Vector2 Normalise() ;

	// Static functions.

	static  float Dot( Vector2 left,  Vector2 right);

	static  float Cross( Vector2 left,  Vector2 right);

	static  float Lerp( float from,  float to,  float ratio);

	static  Vector2 Lerp( Vector2 from,  Vector2 to,  float ratio);

	static  Vector2 TriangleLerp( Vector2 a,  Vector2 b,  Vector2 c,  float ratioX,  float ratioY);

	static  Vector2 BarycetricInterpolator( Vector2 a,  Vector2 b,  Vector2 c,  float ratioX,  float ratioY);

	static  Vector2 Add( Vector2 left,  Vector2 right);

	static  Vector2 Add( Vector2 left,  float scalar);

	static  Vector2 Subtract( Vector2 left,  Vector2 right);

	static  Vector2 Subtract( Vector2 left,  float scalar);

	static  Vector2 Multiply( Vector2 left,  Vector2 right);

	static  Vector2 Multiply( Vector2 left,  float right);

	static  Vector2 Scale( Vector2 left,  float scalar);

	static  Vector2 Divide( Vector2 left,  Vector2 right);

	static  Vector2 Divide( Vector2 left,  float scalar);



	// Operators

	/// Get a vector component given the specified index.
	float operator [] (int i) 
	{
		return (&x)[i];
	}

	 Vector2 operator -()
	{
		return Vector2(-x, -y);
	}

	bool operator ==( Vector2 right) 
	{
		return (fabs(x - right.x) <= FLT_EPSILON)
			&& (fabs(y - right.y) <= FLT_EPSILON);
		//return (x == right.x) && (y == right.y);
	}

	bool operator != (Vector2 right) 
	{
		return !((fabs(x - right.x) <= FLT_EPSILON)
			&& (fabs(y - right.y) <= FLT_EPSILON));
		//return (x != right.x) || (y != right.y);
	}

	 Vector2 operator +=( Vector2 right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	 Vector2 operator *=( Vector2 right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	}

	 Vector2 operator *=( float right)
	{
		x *= right;
		y *= right;
		return *this;
	}

	 Vector2 operator -=( Vector2 right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	 Vector2 operator /=( Vector2 right)
	{
		x /= right.x;
		y /= right.y;
		return *this;
	}

	friend bool operator < (Vector2  left, Vector2  right)
	{
		return (left.x < right.x) || (left.y < right.y);
	}
	friend bool operator > (Vector2  left, Vector2  right)
	{
		return (left.x > right.x) || (left.y > right.y);
	}

	Vector2 operator +(Vector2 right) { return Add(*this, right); }
	Vector2 operator +(int right)  { return Add(*this, right); }
	Vector2 operator -(Vector2 right) { return Subtract(*this, right); }
	Vector2 operator -(float right)  { return Subtract(*this, right); }
	Vector2 operator *(float right) { return Multiply(*this, right); }
	Vector2 operator *(Vector2 right) { return Multiply(*this, right); }
	Vector2 operator /(float right)  { return Divide(*this, right); }
	Vector2 operator /(Vector2 right) { return Divide(*this, right); }

	//Vector2 operator +(Vector2 left, Vector2 right) { return Add(left, right); }
	//Vector2 operator +(Vector2 left, float right) { return Add(left, right); }
	//Vector2 operator -(Vector2 left, Vector2 right) { return Subtract(left, right); }
	//Vector2 operator -(Vector2 left, float right) { return Subtract(left, right); }
	//Vector2 operator *(Vector2 left, Vector2 right) { return Multiply(left, right); }
	//Vector2 operator *(Vector2 left, float right) { return Scale(left, right); }
	//Vector2 operator /(Vector2 left, Vector2 right) { return Divide(left, right); }
	//Vector2 operator /(Vector2 left, float right) { return Divide(left, right); }

	//operator Vector4()
	//{
	//	Vector4 result(x, y, 0.0, 0.0);
	//	return result;
	//}

	//operator Vector3()
	//{
	//	Vector3 result(x, y, 0);
	//	return result;
	//}

	//inline operator Vector2i()
    //{
	//    Vector2i result((int)x, (int)y);
    //	return result;
    //}
};
//}
#endif
