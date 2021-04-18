#ifndef MATRIX_H
#define MATRIX_H

//namespace ath
//{

#include "Vector2i.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

	class Matrix4;
	class Vector4;
	class Vector3;
	class Vector2;
	class Vector2i;
	class Quaternion;
	//class Matrix4;

	/// A column-major order (OpenGL) 4x4 matrix
	class Matrix4
	{
	public:
		//static Matrix4 Identity;

		// Matrix Values

		// Row 0
		float m00;
		float m01;
		float m02;
		float m03;

		// Row 1
		float m10;
		float m11;
		float m12;
		float m13;

		// Row 2
		float m20;
		float m21;
		float m22;
		float m23;

		// Row 3
		float m30;
		float m31;
		float m32;
		float m33;

		// Constructors
		Matrix4();

		Matrix4(
			Vector4 row0,
			Vector4 row1,
			Vector4 row2,
			Vector4 row3
		);

        Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

		// Matrix Rows
		Vector4 GetRow0() const;
		Vector4 GetRow1() const;
		Vector4 GetRow2() const;
		Vector4 GetRow3() const;
		void SetRow0(Vector4 row0);
		void SetRow1(Vector4 row1);
		void SetRow2(Vector4 row2);
		void SetRow3(Vector4 row3);
		// Matrix Rows

		// Matrix Columns

		Vector4 GetColumn0() const;
		Vector4 GetColumn1() const;
		Vector4 GetColumn2() const;
		Vector4 GetColumn3() const;
		void SetColumn0(Vector4 column0);
		void SetColumn1(Vector4 column1);
		void SetColumn2(Vector4 column2);
		void SetColumn3(Vector4 column3);

		// Matrix Columns

		float GetMagnitude() const;
		float GetDeterminant() const;
		static Matrix4 GetIdentity();

		Matrix4 Inverse();
		static Matrix4 Invert(Matrix4 m);

		Vector4 GetDiagonal();

		void ScalarMultiply(Matrix4 input, float scalar);

		static Matrix4 CreateSRTMatrix(Vector3 scale, Vector3 rotationYPR, Vector3 translation);

		static Matrix4 CreateSQTMatrix(Vector3 scale, Quaternion rotation, Vector3 translation);

		void SetTRS(Vector3 translation, Quaternion rotation, Vector3 scale);

		void Scale(float scalar);

		void Scale(Vector3 scale);

		void Scale(Vector4 scale);

		Vector3 GetScale();

		void RotateX(float angleInDegrees);

		void RotateY(float angleInDegrees);

		void RotateZ(float angleInDegrees);

		static Matrix4 RotationYawPitchRoll(float rotationYYawInDegrees, float rotationXPitchInDegrees, float rotationZRollInDegrees);

		static Matrix4 Rotate(Quaternion rotation);

		void Rotate(float rotationXPitchInDegrees, float rotationYYawInDegrees, float rotationZRollInDegrees);

		void Rotate(Vector2 angleXY);

		void Translate(Vector3 translation);

		Vector3 GetTranslation();

		void SetTranslation(Vector3 translation);

		Vector4 GetTranslation4();

		void Translate(Vector4 translation);

		void ShearX(float y, float z);

		void Transpose();

		float* ToArray();

		void FromArray(float* arr);

		// correct multipliers.
		void Multiply(Matrix4 right);

		static Matrix4 Multiply(Matrix4 left, Matrix4 right);

		static Matrix4 Multiply(Matrix4 left, Vector4 right);

		static Vector2 TransformVector(Matrix4 left, Vector2 right);

		static Vector3 TransformVector(Matrix4 left, Vector3 right);

		static Vector4 TransformVector(Matrix4 left, Vector4 right);

		Vector3 TransformVector(Vector3 vector);

		Vector3 TransformPosition(Vector3 vector);

		Vector3 TransformNormal(Vector3 normal);

		static Vector3 TransformNormal(Matrix4 transform, Vector3 normal);

		// Affine Transformations.
		Vector4 TransformSRT(Vector4 vector, Vector3 scale, Vector3 rotationAxis, Vector2 xyRotationDeg, Vector3 translation);

		static Matrix4 CreateScaleMatrix(Vector3 scale);

		static Matrix4 CreateRotationXMatrix(float angle);

		static Matrix4 CreateRotationYMatrix(float angle);

		static Matrix4 CreateRotationZMatrix(float angle);

		static Matrix4 CreateRotatationMatrix(Quaternion rotation);

		static Matrix4 CreateRotatationMatrix(Vector3 rotationAxis, float angle);

		static Matrix4 CreateTranslationMatrix(Vector3 translation);

		static Matrix4 CreateYZFlipMatrix();

		// Projections.

		static Matrix4 Orthographic(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearDistance, float farDistance);

		static Matrix4 Perspective(float fieldOfView, float aspectRatio, float nearDistance, float farDistance);

		static Matrix4 CreateScreenSpaceMatrix(Vector2i halfSize);

		static Matrix4 CreateFrustumMatrix(float left, float right, float bottom, float top, float nearDepth, float farDepth);

		static Matrix4 LookAt(Vector3 eye, Vector3 target, Vector3 up);

		/// Get value in the matrix given the specified index.
		float operator [] (int index) const
		{
			return (&m00)[index];
		}
		/// Get value in the matrix given the specified index.
		float GetValue(int index) const;

		/// Set a value in the matrix given the specified index.
		void SetValue(int index, float value);



		//Matrix4 operator *(Matrix4 right) {
		//	Matrix4 result = static_cast<Matrix4>(Multiply(*this, right));
		//	return result;
		//}
		//Vector4 operator *(Vector4 right) {
		//	Vector4 result = static_cast<Vector4>(TransformVector(*this, right));
		//	return result;
		//}
		//Vector3 operator *(Vector3 right) {
		//	Vector3 result = static_cast<Vector3>(TransformVector(*this, right));
		//	return result;
		//}
		//Vector2 operator *(Vector2 right) {
		//	Vector2 result = static_cast<Vector2>(TransformVector(*this, right));
		//	return result;
		//}
	};

	// Moved to MathHelpers.h:
	
	// inline Matrix4 operator *(Matrix4 left, Matrix4 right) { return Matrix4::Multiply(left, right); }

	//inline Matrix4 operator *=(Matrix4 left, Matrix4 right) {
	//	Matrix4 lhs = static_cast<Matrix4>(left);
	//	return Matrix4::Multiply(lhs, right);
	//}

	//inline Vector4 operator *(Matrix4 left, Vector4 right) { return Matrix4::TransformVector(left, right); }
	//inline Vector3 operator *(Matrix4 left, Vector3 right) { return Matrix4::TransformVector(left, right); }
	//inline Vector2 operator *(Matrix4 left, Vector2 right) { return Matrix4::TransformVector(left, right); }

//}

	// Matrix4 Operators
	Matrix4 operator *(Matrix4 left, Matrix4 right);
	Vector4 operator *(Matrix4 left, Vector4 right);
	Vector3 operator *(Matrix4 left, Vector3 right);
	Vector2 operator *(Matrix4 left, Vector2 right);

	Matrix4 operator *=(Matrix4 left, Matrix4 right);
#endif
