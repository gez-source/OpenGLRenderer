#ifndef QUATERNION_H
#define QUATERNION_H

//namespace ath
//{

#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

	class Vector4;
	class Vector3;
	class Vector2;
	class Vector2i;
	class Quaternion;
	class Matrix4;

	class Quaternion
	{
	public:

		float x;
		float y;
		float z;
		float w;

		//static Quaternion Identity;

		Vector2 xy();

		void xy(Vector2 value);

		Vector3 xyz();

		void xyz(Vector3 value);

		Vector4 xyzw();

		void xyzw(Vector4 value);

		Matrix4 RotationMatrix();

		/**
 * Extracts the rotation part of a transformation matrix and converts it to
 * a quaternion using the magic of maths.
 *
 * More detailed explanation here:
 * http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
 *
 * @param matrix
 *            - the transformation matrix containing the rotation which this
 *            quaternion shall represent.
 */
		static Quaternion FromMatrix(Matrix4 matrix);

		static Quaternion GetIdentity();

		Quaternion();

		Quaternion(float X, float Y, float Z, float W);

		Quaternion(Vector4 vector);

		Quaternion(Vector3 vector, float w);

		Quaternion(Matrix4 matrix);

		/// <summary>
		/// Deconstruct quaternion into axis and angle
		/// </summary>
		/// <param name="axis"></param>
		/// <param name="angle"></param>
		void DeconstructQuaternion(Vector3& axis, float& angle);

		/// <summary>
		/// Deconstruct quaternion into Forward and Up vectors
		/// </summary>
		/// <param name="qOrientation"></param>
		/// <param name="newForwardVector"></param>
		/// <param name="newUpVector"></param>
		void DeconstructQuaternion(Vector3 newForwardVector, Vector3 newUpVector);

		/// <summary>
		/// Deconstruct quaternion into Forward and Up vectors
		/// </summary>
		/// <param name="qOrientation"></param>
		/// <param name="newForwardVector"></param>
		/// <param name="newUpVector"></param>
		static void DeconstructQuaternion(Quaternion qOrientation, Vector3 newForwardVector, Vector3 newUpVector);

		static Quaternion LookRotation(Vector3 lookAt, Vector3 up);

		static Quaternion Euler(float x, float y, float z);

		void ToAxisAngle(Vector3 axis, float angle);

		Vector4 ToAxisAngle();

		static Quaternion Slerp(Quaternion q1, Quaternion q2, float ratio);

		Quaternion Conjugate();

		Quaternion Normalised();

		void Normalise();

		float Magnitude();

		float MagnitudeSquared();

		static Quaternion Add(Quaternion left, Quaternion right);

		static Quaternion Add(Quaternion left, float right);

		static Quaternion Subtract(Quaternion left, Quaternion right);

		static Quaternion Subtract(Quaternion left, float right);

		static Vector4 Multiply(Quaternion left, Vector4 right);

		static Vector3 Multiply(Quaternion left, Vector3 right);

		static Quaternion Multiply(Quaternion left, Quaternion right);

		static Quaternion Scale(Quaternion quaternion, float scale);

		static Quaternion Divide(Quaternion quaternion, float divider);

		//Quaternion operator +(Quaternion left, Quaternion right) { return Add(left, right); }
		//Quaternion operator +(Quaternion left, float right) { return Add(left, right); }
		//Quaternion operator -(Quaternion left, Quaternion right) { return Subtract(left, right); }
		//Quaternion operator -(Quaternion left, float right) { return Subtract(left, right); }
		//Quaternion operator *(Quaternion left, Quaternion right) { return Multiply(left, right); }
		//Vector4 operator *(Quaternion left, Vector4 right) { return Multiply(left, right); }
		//Vector3 operator *(Quaternion left, Vector3 right) { return Multiply(left, right); }
		//Quaternion operator *(Quaternion left, float right) { return Scale(left, right); }
		////Quaternion operator /(Quaternion left, Quaternion right) { return Divide(left, right); } // Is actually possible to divide two quaternions but not sure how.
		//Quaternion operator /(Quaternion left, float right) { return Divide(left, right); }
	};

	// Moved to MathHelpers.h
	//inline Quaternion operator +(Quaternion left, Quaternion right) { return Quaternion::Add(left, right); }
	//inline Quaternion operator +(Quaternion left, float right) { return Quaternion::Add(left, right); }
	//inline Quaternion operator -(Quaternion left, Quaternion right) { return Quaternion::Subtract(left, right); }
	//inline Quaternion operator -(Quaternion left, float right) { return Quaternion::Subtract(left, right); }
	//inline Quaternion operator *(Quaternion left, Quaternion right) { return Quaternion::Multiply(left, right); }
	//inline Vector4 operator *(Quaternion left, Vector4 right) { return Quaternion::Multiply(left, right); }
	//inline Vector3 operator *(Quaternion left, Vector3 right) { return Quaternion::Multiply(left, right); }
	//inline Quaternion operator *(Quaternion left, float right) { return Quaternion::Scale(left, right); }
	//inline Quaternion operator /(Quaternion left, float right) { return Quaternion::Divide(left, right); }

	// Quaternion Operators
	Quaternion operator +(Quaternion left, Quaternion right);
	Quaternion operator +(Quaternion left, float right);
	Quaternion operator -(Quaternion left, Quaternion right);
	Quaternion operator -(Quaternion left, float right);
	Quaternion operator *(Quaternion left, Quaternion right);
	Vector4 operator *(Quaternion left, Vector4 right);
	Vector3 operator *(Quaternion left, Vector3 right);
	Quaternion operator *(Quaternion left, float right);
	Quaternion operator /(Quaternion left, float right);

//}
#endif
