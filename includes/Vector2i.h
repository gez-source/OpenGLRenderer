#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "MathExt.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"

//namespace ath
//{
class Vector4;
class Vector3;
class Vector2;
class Vector2i;

class Vector2i
{
public:
	int x;
	int y;

	static Vector2i Zero;
	static Vector2i One;
	static Vector2i UnitX;
	static Vector2i UnitY;

	Vector2i();
	Vector2i(int X, int Y);
	Vector2i(float X, float Y);

	Vector2 xy() ;
	Vector3 xyz() ;
	Vector4 xyzw() ;

	void xy(Vector2 xy);
	void xyz(Vector3 xyz);
	Vector3 xyw();
	void xyw(Vector3 xyw);

	Vector2i Translate(Vector2i translation) ;
	Vector2i Scale(Vector2i s) ;
	Vector2i Scale(float s) ;
	Vector2i RotateXY(float xAngleDegrees, float yAngleDegrees) ;

	float MagnitudeSquared() ;
	float Magnitude() ;
	float GetMagnitudeSquared() ;
	float GetMagnitude() ;

	// Static functions.
	static float Dot(Vector2i left, Vector2i right);
	static float Cross(Vector2i left, Vector2i right);
	static float Lerp(float from, float to, float ratio);
	static Vector2i Lerp(Vector2i from, Vector2i to, float ratio);
	static Vector2i TriangleLerp(Vector2i a, Vector2i b, Vector2i c, float ratioX, float ratioY);
	static Vector2i BarycetricInterpolator(Vector2i a, Vector2i b, Vector2i c, float ratioX, float ratioY);
	static Vector2i Add(Vector2i left, Vector2i right);
	static Vector2i Add(Vector2i left, int scalar);
	static Vector2i Subtract(Vector2i left, Vector2i right);
	static Vector2i Subtract(Vector2i left, int scalar);
	static Vector2i Subtract(float scalar, Vector2i right);
	static Vector2i Multiply(Vector2i left, Vector2i right);
	static Vector2i Multiply(Vector2i left, float right);
	static Vector2i Scale(Vector2i left, float scalar);
	static Vector2i Scale(Vector2i left, int scalar);
	static Vector2i Divide(Vector2i left, Vector2i right);
	static Vector2i Divide(Vector2i left, int scalar);

	// Operators

	/// Get a vector component given the specified index.
	int operator [] (int i) 
	{
		return (&x)[i];
	}

	Vector2i operator -() 
	{
		return Vector2i(-x, -y);
	}

	bool operator ==(Vector2i right) 
	{
		return (x == right.x) && (y == right.y);
	}

	bool operator !=(Vector2i right) 
	{
		return (x != right.x) || (y != right.y);
	}

	Vector2i operator +=(Vector2i right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2i operator *=(Vector2i right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	}

	Vector2i operator *=(float right)
	{
		x *= right;
		y *= right;
		return *this;
	}

	Vector2i operator -=(Vector2i right)
	{
		x -= right.x;
		y -= right.y;
		return *this;
	}

	Vector2i operator /=(Vector2i right)
	{
		x /= right.x;
		y /= right.y;
		return *this;
	}

	friend bool operator < (Vector2i left, Vector2i right)
	{
		return (left.x < right.x) || (left.y < right.y);
	}
	friend bool operator > (Vector2i left, Vector2i right)
	{
		return (left.x > right.x) || (left.y > right.y);
	}

	Vector2i operator +(Vector2i right) { return Add(*this, right); }
	Vector2i operator +(int right){ return Add(*this, right); }
	Vector2i operator -(Vector2i right){ return Subtract(*this, right); }
	Vector2i operator -(float right) { return Subtract(*this, right); }
	Vector2i operator *(float right){ return Multiply(*this, right); }
	Vector2i operator *(Vector2i right){ return Multiply(*this, right); }
	Vector2i operator /(float right){ return Divide(*this, right); }
	Vector2i operator /(Vector2i right){ return Divide(*this, right); }

	//Vector2i operator +(Vector2i left, Vector2i right) { return Add(left, right); }
	//Vector2i operator +(Vector2i left, int right) { return Add(left, right); }
	//Vector2i operator -(Vector2i left, Vector2i right) { return Subtract(left, right); }
	//Vector2i operator -(float left, Vector2i right) { return Subtract(left, right); }
	//Vector2i operator -(Vector2i left, int right) { return Subtract(left, right); }
	//Vector2i operator *(Vector2i left, Vector2i right) { return Multiply(left, right); }
	//Vector2i operator *(Vector2i left, float right) { return Scale(left, right); }
	//Vector2i operator *(Vector2i left, int right) { return Scale(left, right); }
	//Vector2i operator /(Vector2i left, Vector2i right) { return Divide(left, right); }
	//Vector2i operator /(Vector2i left, int right) { return Divide(left, right); }


	//inline operator Vector4()
	//{
	//	Vector4 result(x, y, 0.0, 0.0);
	//	return result;
	//}

	//inline operator Vector3()
	//{
	//	Vector3 result(x, y, 0);
	//	return result;
	//}
    //operator Vector2();
	//inline operator Vector2()
    //{
	//  Vector2 result((int)x, (int)y);
    //  return result;
    //}
};
//}

#endif	
