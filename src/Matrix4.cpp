
#include <algorithm>
#include <iostream>
#include "Matrix4.h"

//namespace ath
//{
	// Define and Initialise static members of class Matrix4.
	//Matrix4 Matrix4::Identity = Matrix4::GetIdentity();

	Matrix4::Matrix4()
	{
		m00 = 0;
		m01 = 0;
		m02 = 0;
		m03 = 0;
		m10 = 0;
		m11 = 0;
		m12 = 0;
		m13 = 0;
		m20 = 0;
		m21 = 0;
		m22 = 0;
		m23 = 0;
		m30 = 0;
		m31 = 0;
		m32 = 0;
		m33 = 0;
	}

    Matrix4::Matrix4(float v00, float v01, float v02, float v03,
            float v10, float v11, float v12, float v13,
            float v20, float v21, float v22, float v23,
            float v30, float v31, float v32, float v33)
    {
		this->m00 = v00;
		this->m01 = v01;
		this->m02 = v02;
		this->m03 = v03;
		this->m10 = v10;
		this->m11 = v11;
		this->m12 = v12;
		this->m13 = v13;
		this->m20 = v20;
		this->m21 = v21;
		this->m22 = v22;
		this->m23 = v23;
		this->m30 = v30;
		this->m31 = v31;
		this->m32 = v32;
		this->m33 = v33;
    }

	Matrix4::Matrix4(Vector4 row0, Vector4 row1, Vector4 row2, Vector4 row3)
	{
		m00 = row0.x;
		m01 = row0.y;
		m02 = row0.z;
		m03 = row0.w;

		m10 = row1.x;
		m11 = row1.y;
		m12 = row1.z;
		m13 = row1.w;

		m20 = row2.x;
		m21 = row2.y;
		m22 = row2.z;
		m23 = row2.w;

		m30 = row3.x;
		m31 = row3.y;
		m32 = row3.z;
		m33 = row3.w;
	}

	// Matrix Rows
	void Matrix4::SetRow0(Vector4 row0)
	{
		m00 = row0.x;
		m01 = row0.y;
		m02 = row0.z;
		m03 = row0.w;
	}

	void Matrix4::SetRow1(Vector4 row1)
	{
		m10 = row1.x;
		m11 = row1.y;
		m12 = row1.z;
		m13 = row1.w;
	}

	void Matrix4::SetRow2(Vector4 row2)
	{
		m20 = row2.x;
		m21 = row2.y;
		m22 = row2.z;
		m23 = row2.w;
	}

	void Matrix4::SetRow3(Vector4 row3)
	{
		m30 = row3.x;
		m31 = row3.y;
		m32 = row3.z;
		m33 = row3.w;
	}

	Vector4 Matrix4::GetRow0() const
	{
		Vector4 row0(m00, m01, m02, m03);
		return row0;
	}

	Vector4 Matrix4::GetRow1()const
	{
		Vector4 row1(m10, m11, m12, m13);
		return row1;
	}

	Vector4 Matrix4::GetRow2()const
	{
		Vector4 row2(m20, m21, m22, m23);
		return row2;
	}

	Vector4 Matrix4::GetRow3()const
	{
		Vector4 row3(m30, m31, m32, m33);
		return row3;
	}

	// Matrix Columns

	Vector4 Matrix4::GetColumn0()const
	{
		Vector4 column0(m00, m10, m20, m30);
		return column0;
	}

	Vector4 Matrix4::GetColumn1()const
	{
		Vector4 column1(m01, m11, m21, m31);
		return column1;
	}

	Vector4 Matrix4::GetColumn2()const
	{
		Vector4 column2(m02, m12, m22, m32);
		return column2;
	}

	Vector4 Matrix4::GetColumn3() const
	{
		Vector4 column3(m03, m13, m23, m33);
		return column3;
	}

	void Matrix4::SetColumn0(Vector4 column0)
	{
		m00 = column0.x;
		m10 = column0.y;
		m20 = column0.z;
		m30 = column0.w;
	}

	void Matrix4::SetColumn1(Vector4 column1)
	{
		m01 = column1.x;
		m11 = column1.y;
		m21 = column1.z;
		m31 = column1.w;
	}

	void Matrix4::SetColumn2(Vector4 column2)
	{
		m02 = column2.x;
		m12 = column2.y;
		m22 = column2.z;
		m32 = column2.w;
	}

	void Matrix4::SetColumn3(Vector4 column3)
	{
		m03 = column3.x;
		m13 = column3.y;
		m23 = column3.z;
		m33 = column3.w;
	}

	// Matrix Columns

	Matrix4 Matrix4::GetIdentity()
	{
		Matrix4 identity(
			Vector4::UnitX,
			Vector4::UnitY,
			Vector4::UnitZ,
			Vector4::UnitW
		);
		return identity;
	}

	float Matrix4::GetMagnitude() const
	{
		return GetDeterminant();
	}

	float Matrix4::GetDeterminant() const
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();
		Vector4 Row3 = GetRow3();

		return

			(Row0.x * Row1.y * Row2.z * Row3.w) - (Row0.x * Row1.y * Row2.w * Row3.z)
			+ (Row0.x * Row1.z * Row2.w * Row3.y) - (Row0.x * Row1.z * Row2.y * Row3.w)
			+ (Row0.x * Row1.w * Row2.y * Row3.z) - (Row0.x * Row1.w * Row2.z * Row3.y)

			- (Row0.y * Row1.z * Row2.w * Row3.x) + (Row0.y * Row1.z * Row2.x * Row3.w)
			- (Row0.y * Row1.w * Row2.x * Row3.z) + (Row0.y * Row1.w * Row2.z * Row3.x)
			- (Row0.y * Row1.x * Row2.z * Row3.w) + (Row0.y * Row1.x * Row2.w * Row3.z)

			+ (Row0.z * Row1.w * Row2.x * Row3.y) - (Row0.z * Row1.w * Row2.y * Row3.x)
			+ (Row0.z * Row1.x * Row2.y * Row3.w) - (Row0.z * Row1.x * Row2.w * Row3.y)
			+ (Row0.z * Row1.y * Row2.w * Row3.x) - (Row0.z * Row1.y * Row2.x * Row3.w)

			- (Row0.w * Row1.x * Row2.y * Row3.z) + (Row0.w * Row1.x * Row2.z * Row3.y)
			- (Row0.w * Row1.y * Row2.z * Row3.x) + (Row0.w * Row1.y * Row2.x * Row3.z)
			- (Row0.w * Row1.z * Row2.x * Row3.y) + (Row0.w * Row1.z * Row2.y * Row3.x);
	}

	Matrix4 Matrix4::Invert(Matrix4 m)
	{
		return m.Inverse();
	}

	Matrix4 Matrix4::Inverse()
	{
		Matrix4 result;
		float determinant;
		int i;

		determinant = GetDeterminant();

		if (determinant == 0)
		{
			std::cout << ("Matrix can not be inverted.");
			exit(1);
		}
		else
		{
			determinant = 1.0f / determinant;

            result.m00 = determinant * (m11*m22*m33 + m12*m23*m31 + m13*m21*m32 - m13*m22*m31 - m12*m21*m33 - m11*m23*m32);
            result.m01 = determinant * (-m01*m22*m33 - m02*m23*m31 - m03*m21*m32 + m03*m22*m31 + m02*m21*m33 + m01*m23*m32);
            result.m02 = determinant * (m01*m12*m33 + m02*m13*m31 + m03*m11*m32 - m03*m12*m31 - m02*m11*m33 - m01*m13*m32);
            result.m03 = determinant * (-m01*m12*m23 - m02*m13*m21 - m03*m11*m22 + m03*m12*m21 + m02*m11*m23 + m01*m13*m22);

            result.m10 = determinant * (-m10*m22*m33 - m12*m23*m30 - m13*m20*m32 + m13*m22*m30 + m12*m20*m33 + m10*m23*m32);
            result.m11 = determinant * (m00*m22*m33 + m02*m23*m30 + m03*m20*m32 - m03*m22*m30 - m02*m20*m33 - m00*m23*m32);
            result.m12 = determinant * (-m00*m12*m33 - m02*m13*m30 - m03*m10*m32 + m03*m12*m30 + m02*m10*m33 + m00*m13*m32);
            result.m13 = determinant * (m00*m12*m23 + m02*m13*m20 + m03*m10*m22 - m03*m12*m20 - m02*m10*m23 - m00*m13*m22);

            result.m20 = determinant * (m10*m21*m33 + m11*m23*m30 + m13*m20*m31 - m13*m21*m30 - m11*m20*m33 - m10*m23*m31);
            result.m21 = determinant * (-m00*m21*m33 - m01*m23*m30 - m03*m20*m31 + m03*m21*m30 + m01*m20*m33 + m00*m23*m31);
            result.m22 = determinant * (m00*m11*m33 + m01*m13*m30 + m03*m10*m31 - m03*m11*m30 - m01*m10*m33 - m00*m13*m31);
            result.m23 = determinant * (-m00*m11*m23 - m01*m13*m20 - m03*m10*m21 + m03*m11*m20 + m01*m10*m23 + m00*m13*m21);

            result.m30 = determinant * (-m10*m21*m32 - m11*m22*m30 - m12*m20*m31 + m12*m21*m30 + m11*m20*m32 + m10*m22*m31);
            result.m31 = determinant * (m00*m21*m32 + m01*m22*m30 + m02*m20*m31 - m02*m21*m30 - m01*m20*m32 - m00*m22*m31);
            result.m32 = determinant * (-m00*m11*m32 - m01*m12*m30 - m02*m10*m31 + m02*m11*m30 + m01*m10*m32 + m00*m12*m31);
            result.m33 = determinant * (m00*m11*m22 + m01*m12*m20 + m02*m10*m21 - m02*m11*m20 - m01*m10*m22 - m00*m12*m21);
		}


		return result;
	}

	Vector4 Matrix4::GetDiagonal()
	{
		// [m00]  m01   m02   m03
		//  m10  [m11]  m12   m13
		//  m20   m21  [m22]  m23
		//  m30   m31   m32  [m33]

		return Vector4(m00, m11, m22, m33);
	}

	void Matrix4::ScalarMultiply(Matrix4 input, float scalar)
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();
		Vector4 Row3 = GetRow3();

		Vector4 res0(Row0.x * scalar, Row0.y * scalar, Row0.z * scalar, Row0.w * scalar);
		Vector4 res1(Row1.x * scalar, Row1.y * scalar, Row1.z * scalar, Row1.w * scalar);
		Vector4 res2(Row2.x * scalar, Row2.y * scalar, Row2.z * scalar, Row2.w * scalar);
		Vector4 res3(Row3.x * scalar, Row3.y * scalar, Row3.z * scalar, Row3.w * scalar);

		input.SetRow0(res0);
		input.SetRow1(res1);
		input.SetRow2(res2);
		input.SetRow3(res3);
	}

	Matrix4 Matrix4::CreateSRTMatrix(Vector3 scale, Vector3 rotationYPR, Vector3 translation)
	{
		// Manual SRT 4x4 Matrix

		Matrix4 matrixSRT = Matrix4::GetIdentity();

		matrixSRT.Scale(scale);
		matrixSRT = Matrix4::Multiply(matrixSRT, RotationYawPitchRoll(rotationYPR.y, rotationYPR.x, rotationYPR.z));
		matrixSRT.Translate(translation);

		return matrixSRT;
	}
	Matrix4 Matrix4::CreateSQTMatrix(Vector3 scale, Quaternion rotation, Vector3 translation)
	{
		// Manual SRT 4x4 Matrix

		Matrix4 matrixSRT = Matrix4::GetIdentity();

		matrixSRT.Scale(scale);
		matrixSRT = Matrix4::Multiply(matrixSRT, CreateRotatationMatrix(rotation));
		matrixSRT.Translate(translation);

		return matrixSRT;
	}
	void Matrix4::SetTRS(Vector3 translation, Quaternion rotation, Vector3 scale)
	{
		Matrix4 result = *this;
		Matrix4 scaleMatrix = CreateScaleMatrix(scale);
		Matrix4 rotationMatrix = CreateRotatationMatrix(rotation);
		Matrix4 translationMatrix = CreateTranslationMatrix(translation);

        result = Matrix4::Multiply(result, scaleMatrix);
        result = Matrix4::Multiply(result, rotationMatrix);
        result = Matrix4::Multiply(result, translationMatrix);

		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	void Matrix4::Scale(float scalar)
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();
		Vector4 Row3 = GetRow3();

		Row0 = Vector4::Multiply(Row0, Vector4(scalar, 1, 1, 1));
		Row1 = Vector4::Multiply(Row1, Vector4(1, scalar, 1, 1));
		Row2 = Vector4::Multiply(Row2, Vector4(1, 1, scalar, 1));
		Row3.w = 1.0f;

		SetRow0(Row0);
		SetRow1(Row1);
		SetRow2(Row2);
		SetRow3(Row3);
	}

	void Matrix4::Scale(Vector3 scale)
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();
		Vector4 Row3 = GetRow3();

        Row0 = Vector4::Multiply(Row0, Vector4(scale.x, 1, 1, 1));
		Row1 = Vector4::Multiply(Row1, Vector4(1, scale.y, 1, 1));
		Row2 = Vector4::Multiply(Row2, Vector4(1, 1, scale.z, 1));
		Row3.w = 1.0f;

		SetRow0(Row0);
		SetRow1(Row1);
		SetRow2(Row2);
		SetRow3(Row3);
	}

	void Matrix4::Scale(Vector4 scale)
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();
		Vector4 Row3 = GetRow2();

		Row0 = Vector4::Multiply(Row0, Vector4(scale.x, 1, 1, 1));
		Row1 = Vector4::Multiply(Row1, Vector4(1, scale.y, 1, 1));
		Row2 = Vector4::Multiply(Row2, Vector4(1, 1, scale.z, 1));
		Row3 = Vector4::Multiply(Row3, Vector4(1, 1, 1, scale.w));

		SetRow0(Row0);
		SetRow1(Row1);
		SetRow2(Row2);
		SetRow3(Row3);
	}

	Vector3 Matrix4::GetScale()
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow1();
		Vector4 Row2 = GetRow2();

		Vector3 scale(Row0.x, Row1.y, Row2.z);

		return scale;
	}

	void Matrix4::RotateX(float angleInDegrees)
	{
		float angleInRadians = Deg2Rad * angleInDegrees;
		Matrix4 rotationMatrix = GetIdentity();
		rotationMatrix.SetRow0(Vector4::UnitX);
		rotationMatrix.SetRow1(Vector4(0, std::cos(angleInRadians), std::sin(angleInRadians), 0.0f));
		rotationMatrix.SetRow2(Vector4(0, -std::sin(angleInRadians), std::cos(angleInRadians), 0.0f));
		rotationMatrix.SetRow3(Vector4::UnitW);

		Matrix4 result;
		result = Multiply(*this, rotationMatrix);
		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	void Matrix4::RotateY(float angleInDegrees)
	{
		float angleInRadians = Deg2Rad * angleInDegrees;
		Matrix4 rotationMatrix = GetIdentity();
		rotationMatrix.SetRow0(Vector4(std::cos(angleInRadians), 0, -std::sin(angleInRadians), 0.0f));
		rotationMatrix.SetRow1(Vector4::UnitY);
		rotationMatrix.SetRow2(Vector4(std::sin(angleInRadians), 0, std::cos(angleInRadians), 0.0f));
		rotationMatrix.SetRow3(Vector4::UnitW);

		Matrix4 result;
		result = Multiply(*this, rotationMatrix);
		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	void Matrix4::RotateZ(float angleInDegrees)
	{
		float angleInRadians = Deg2Rad * angleInDegrees;
		Matrix4 rotationMatrix = GetIdentity();
		rotationMatrix.SetRow0(Vector4(std::cos(angleInRadians), std::sin(angleInRadians), 0.0f, 0.0f));
		rotationMatrix.SetRow1(Vector4(-std::sin(angleInRadians), std::cos(angleInRadians), 0.0f, 0.0f));
		rotationMatrix.SetRow2(Vector4::UnitZ);
		rotationMatrix.SetRow3(Vector4::UnitW);

		Matrix4 result;
		result = Multiply(*this, rotationMatrix);
		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	Matrix4 Matrix4::RotationYawPitchRoll(float rotationYYawInDegrees, float rotationXPitchInDegrees, float rotationZRollInDegrees)
	{
		Matrix4 rotationMatrix = Matrix4::GetIdentity();
		rotationMatrix.RotateX(rotationXPitchInDegrees);
		rotationMatrix.RotateY(rotationYYawInDegrees);
		rotationMatrix.RotateZ(rotationZRollInDegrees);
		return rotationMatrix;
	}

	void Matrix4::Rotate(float rotationXPitchInDegrees, float rotationYYawInDegrees, float rotationZRollInDegrees)
	{
		Quaternion rotation = Quaternion::Euler(rotationXPitchInDegrees, rotationYYawInDegrees, rotationZRollInDegrees);
		Matrix4 result(GetRow0(), GetRow1(), GetRow2(), GetRow3());
		result = rotation.RotationMatrix() * result;
		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	Matrix4 Matrix4::Rotate(Quaternion rotation)
	{
		return CreateRotatationMatrix(rotation);
	}

	void Matrix4::Rotate(Vector2 angleXY)
	{
		Matrix4 that = *this;
		Matrix4 result;
		Matrix4 rotationMatrixX = CreateRotationXMatrix(angleXY.x);
		Matrix4 rotationMatrixY = CreateRotationYMatrix(angleXY.x);
		result = Multiply(that, rotationMatrixX);
		result = Multiply(result, rotationMatrixY);
		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	void Matrix4::Translate(Vector3 translation)
	{
		Vector4 Row3 = GetRow3();

        Row3 = Vector4::Add(Row3, Vector4(translation, 1.0f));
		//Row3 += Vector4(translation, 0.0f);
		Row3.w = 1.0f;

		SetRow3(Row3);
	}

	Vector3 Matrix4::GetTranslation()
	{
		Vector4 Row3 = GetRow3();
		return Row3.xyz();
	}

	void Matrix4::SetTranslation(Vector3 translation)
	{
		Vector4 Row3 = Vector4(translation, 1.0f);
		SetRow3(Row3);
	}

	Vector4 Matrix4::GetTranslation4()
	{
		Vector4 Row3 = GetRow3();
		return Row3;
	}

	void Matrix4::Translate(Vector4 translation)
	{
		Vector4 Row3 = GetRow3();
		//Row3 += translation;
		Row3 = Vector4::Add(Row3, translation);
		SetRow3(Row3);
	}

	void Matrix4::ShearX(float y, float z)
	{
		Vector4 Row0 = GetRow0();
		Vector4 Row1 = GetRow0();
		Vector4 Row2 = GetRow0();

		Row0 = Vector4(1, y, z, Row0.w);
		Row1 = Vector4(Vector3::UnitY, Row0.w);
		Row2 = Vector4(Vector3::UnitZ, Row0.w);

		SetRow3(Row0);
		SetRow3(Row1);
		SetRow3(Row2);
	}

	void Matrix4::Transpose()
	{
		Matrix4 m;

		m.SetRow0(GetColumn0());
		m.SetRow1(GetColumn1());
		m.SetRow2(GetColumn2());
		m.SetRow3(GetColumn3());

		SetRow0(m.GetRow0());
		SetRow1(m.GetRow1());
		SetRow2(m.GetRow2());
		SetRow3(m.GetRow3());
	}

	float* Matrix4::ToArray()
	{
		float* result = new float[16]
		{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		};
		return result;
	}

	void Matrix4::FromArray(float* arr)
	{
		m00 = arr[0];
		m01 = arr[1];
		m02 = arr[2];
		m03 = arr[3];

		m10 = arr[4];
		m11 = arr[5];
		m12 = arr[6];
		m13 = arr[7];

		m20 = arr[8];
		m21 = arr[9];
		m22 = arr[10];
		m23 = arr[11];

		m30 = arr[12];
		m31 = arr[13];
		m32 = arr[14];
		m33 = arr[15];
	}

	void Matrix4::Multiply(Matrix4 right)
	{
		Matrix4 result = Multiply(*this, right);

		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
		SetRow3(result.GetRow3());
	}

	Matrix4 Matrix4::Multiply(Matrix4 left, Matrix4 right)
	{
		Matrix4 result;

		result.m00 = left.m00 * right.m00 + left.m10 * right.m01 + left.m20 * right.m02 + left.m30 * right.m03;
		result.m01 = left.m01 * right.m00 + left.m11 * right.m01 + left.m21 * right.m02 + left.m31 * right.m03;
		result.m02 = left.m02 * right.m00 + left.m12 * right.m01 + left.m22 * right.m02 + left.m32 * right.m03;
		result.m03 = left.m03 * right.m00 + left.m13 * right.m01 + left.m23 * right.m02 + left.m33 * right.m03;
		result.m10 = left.m00 * right.m10 + left.m10 * right.m11 + left.m20 * right.m12 + left.m30 * right.m13;
		result.m11 = left.m01 * right.m10 + left.m11 * right.m11 + left.m21 * right.m12 + left.m31 * right.m13;
		result.m12 = left.m02 * right.m10 + left.m12 * right.m11 + left.m22 * right.m12 + left.m32 * right.m13;
		result.m13 = left.m03 * right.m10 + left.m13 * right.m11 + left.m23 * right.m12 + left.m33 * right.m13;
		result.m20 = left.m00 * right.m20 + left.m10 * right.m21 + left.m20 * right.m22 + left.m30 * right.m23;
		result.m21 = left.m01 * right.m20 + left.m11 * right.m21 + left.m21 * right.m22 + left.m31 * right.m23;
		result.m22 = left.m02 * right.m20 + left.m12 * right.m21 + left.m22 * right.m22 + left.m32 * right.m23;
		result.m23 = left.m03 * right.m20 + left.m13 * right.m21 + left.m23 * right.m22 + left.m33 * right.m23;
		result.m30 = left.m00 * right.m30 + left.m10 * right.m31 + left.m20 * right.m32 + left.m30 * right.m33;
		result.m31 = left.m01 * right.m30 + left.m11 * right.m31 + left.m21 * right.m32 + left.m31 * right.m33;
		result.m32 = left.m02 * right.m30 + left.m12 * right.m31 + left.m22 * right.m32 + left.m32 * right.m33;
		result.m33 = left.m03 * right.m30 + left.m13 * right.m31 + left.m23 * right.m32 + left.m33 * right.m33;

		return result;
	}

	Matrix4 Matrix4::Multiply(Matrix4 left, Vector4 right)
	{
		Matrix4 result = GetIdentity();

		result.m00 = (left.m00 * right.x);
		result.m01 = (left.m00 * right.y);
		result.m02 = (left.m00 * right.z);
		result.m03 = (left.m00 * right.w);

		result.m10 = (left.m10 * right.x);
		result.m11 = (left.m10 * right.y);
		result.m12 = (left.m10 * right.z);
		result.m13 = (left.m10 * right.w);

		result.m20 = (left.m20 * right.x);
		result.m21 = (left.m20 * right.y);
		result.m22 = (left.m20 * right.z);
		result.m23 = (left.m20 * right.w);

		result.m30 = (left.m30 * right.x);
		result.m31 = (left.m30 * right.y);
		result.m32 = (left.m30 * right.z);
		result.m33 = (left.m30 * right.w);

		return result;
	}

	Vector2 Matrix4::TransformVector(Matrix4 left, Vector2 right)
	{
		Vector2 result
		(
			Vector2::Dot(right, left.GetColumn0().xy()),
			Vector2::Dot(right, left.GetColumn1().xy())
		); // Neater to do with Dot Products.
		return result;
	}

	Vector3 Matrix4::TransformVector(Matrix4 left, Vector3 right)
	{
		Vector3 result; // Neater to do with Dot Products.
		result.x = Vector4::Dot(right.xyzw(), left.GetColumn0());
		result.y = Vector4::Dot(right.xyzw(), left.GetColumn1());
		result.z = Vector4::Dot(right.xyzw(), left.GetColumn2());
		return result;
	}

	Vector4 Matrix4::TransformVector(Matrix4 left, Vector4 right)
	{
		Vector4 result(
			Vector4::Dot(right, left.GetColumn0()),
			Vector4::Dot(right, left.GetColumn1()),
			Vector4::Dot(right, left.GetColumn2()),
			Vector4::Dot(right, left.GetColumn3())
		); // Neater to do with Dot Products.
		return result;
	}

	Vector3 Matrix4::TransformVector(Vector3 vector)
	{
        Vector3 result(
			Vector4::Dot(vector, this->GetColumn0()),
			Vector4::Dot(vector, this->GetColumn1()),
			Vector4::Dot(vector, this->GetColumn2())
		); // Neater to do with Dot Products.
		return result;
	}

	Vector3 Matrix4::TransformPosition(Vector3 vector)
	{
		Vector3 translation = GetTranslation();
		Vector3 result(
			Vector3::Dot(vector, Vector3(this->GetColumn0())) + translation.x,
			Vector3::Dot(vector, Vector3(this->GetColumn1())) + translation.y,
			Vector3::Dot(vector, Vector3(this->GetColumn2())) + translation.z
		); // Neater to do with Dot Products.		
		return result;
	}

	Vector3 Matrix4::TransformNormal(Vector3 normal)
	{
		Vector3 result;
		Matrix4 transform = *this;
		Matrix4 m = transform.Inverse();

		Vector3 r0(m.GetRow0());
		Vector3 r1(m.GetRow1());
		Vector3 r2(m.GetRow2());

		Vector3 tmp
		(
			Vector3::Dot(normal, r0),
			Vector3::Dot(normal, r1),
			Vector3::Dot(normal, r2)
		);

		result = tmp;

		return result;
	}

	Vector3 Matrix4::TransformNormal(Matrix4 transform, Vector3 normal)
	{
		Vector3 result;
		Matrix4 m = static_cast<Matrix4>(transform).Inverse();

		Vector3 r0(m.GetRow0());
		Vector3 r1(m.GetRow1());
		Vector3 r2(m.GetRow2());

		Vector3 tmp
		(
			Vector3::Dot(normal, r0),
			Vector3::Dot(normal, r1),
			Vector3::Dot(normal, r2)
		);

		result = tmp;

		return result;
	}

	Vector4 Matrix4::TransformSRT(Vector4 vector, Vector3 scale, Vector3 rotationAxis, Vector2 xyRotationDeg, Vector3 translation)
	{
		// Manual SRT 4x4 Matrix

		Vector4 result;
		Vector3 axis = static_cast<Vector3>(rotationAxis).Normalise();

		// Creates a rotation about rotationAxis given two angles specified in xyRotationDeg.
		Vector2 unitCircleRatioX(
			std::cos(-xyRotationDeg.x),
			std::sin(-xyRotationDeg.x)
		);
		Vector2 unitCircleRatioY(
			std::cos(-xyRotationDeg.y),
			std::sin(-xyRotationDeg.y)
		);

		Vector2 t(1.0f - unitCircleRatioX.x, 1.0f - unitCircleRatioY.x);

		Vector4 row0Rotation(
			((t.y * axis.x * axis.x) + unitCircleRatioY.x) * ((t.x * axis.x * axis.x) + unitCircleRatioX.x),
			((t.y * axis.x * axis.y - unitCircleRatioY.y * axis.z)) * ((t.x * axis.x * axis.y - unitCircleRatioX.y * axis.z)),
			((t.y * axis.x * axis.z + unitCircleRatioY.y * axis.y)) * ((t.x * axis.x * axis.z + unitCircleRatioX.y * axis.y)),
			0.0f
		);
		Vector4 row1Rotation(
			((t.y * axis.x * axis.y + unitCircleRatioY.y * axis.z)) * ((t.x * axis.x * axis.y + unitCircleRatioX.y * axis.z)),
			((t.y * axis.y * axis.y + unitCircleRatioY.x)) * ((t.x * axis.y * axis.y + unitCircleRatioX.x)),
			((t.y * axis.y * axis.z - unitCircleRatioY.y * axis.x)) * ((t.x * axis.y * axis.z - unitCircleRatioX.y * axis.x)),
			0.0f
		);
		Vector4 row2Rotation(
			((t.y * axis.x * axis.z - unitCircleRatioY.y * axis.y)) * ((t.x * axis.x * axis.z - unitCircleRatioX.y * axis.y)),
			((t.y * axis.y * axis.z + unitCircleRatioY.y * axis.x)) * ((t.x * axis.y * axis.z + unitCircleRatioX.y * axis.x)),
			((t.y * axis.z * axis.z + unitCircleRatioY.x)) * ((t.x * axis.z * axis.z + unitCircleRatioX.x)),
			0.0f
		);

		// Create the SRT Matrix.
		Matrix4 scaleMatrix = Matrix4(
			Vector4(scale.x, 0, 0, 0),
			Vector4(0, scale.y, 0, 0),
			Vector4(0, 0, scale.z, 0),
			Vector4(0, 0, 0, 1.0)
		);

		Matrix4 rotationMatrix = Matrix4(
			row0Rotation,
			row1Rotation,
			row2Rotation,
			Vector4(translation, 1.0f)
		);

		Matrix4 matrixSRT = Matrix4::Multiply(scaleMatrix, rotationMatrix);

		result = Matrix4::TransformVector(matrixSRT, vector);

		return result;
	}

	Matrix4 Matrix4::CreateScaleMatrix(Vector3 scale)
	{
		Matrix4 scaleMatrix = GetIdentity();
		scaleMatrix.SetRow0(Vector4::UnitX * scale.x);
		scaleMatrix.SetRow1(Vector4::UnitY * scale.y);
		scaleMatrix.SetRow2(Vector4::UnitZ * scale.z);
		scaleMatrix.SetRow3(Vector4::UnitW);
		return scaleMatrix;
	}

	Matrix4 Matrix4::CreateRotationXMatrix(float angle)
	{
		return CreateRotatationMatrix(Vector3::UnitX, angle);
	}

	Matrix4 Matrix4::CreateRotationYMatrix(float angle)
	{
		return CreateRotatationMatrix(Vector3::UnitY, angle);
	}

	Matrix4 Matrix4::CreateRotationZMatrix(float angle)
	{
		return CreateRotatationMatrix(Vector3::UnitZ, angle);
	}

	Matrix4 Matrix4::CreateRotatationMatrix(Quaternion rotation)
	{
		Vector3 axis;
		float angle;
		rotation.DeconstructQuaternion(axis, angle);
		return CreateRotatationMatrix(axis, angle);
	}

	Matrix4 Matrix4::CreateRotatationMatrix(Vector3 rotationAxis, float angle)
	{
		float cos = std::cos(-angle);
		float sin = std::sin(-angle);
		float t = 1.0f - cos;

		Vector3 axis = rotationAxis;
		axis.Normalise();

		Matrix4 result = GetIdentity();
		result.SetRow0(Vector4(
			(t * axis.x * axis.x) + cos,
			(t * axis.x * axis.y - sin * axis.z),
			(t * axis.x * axis.z + sin * axis.y),
			0.0f
		));
		result.SetRow1(Vector4(
			(t * axis.x * axis.y + sin * axis.z),
			(t * axis.y * axis.y + cos),
			(t * axis.y * axis.z - sin * axis.x),
			0.0f
		));
		result.SetRow2(Vector4(
			(t * axis.x * axis.z - sin * axis.y),
			(t * axis.y * axis.z + sin * axis.x),
			(t * axis.z * axis.z + cos),
			0.0f
		));
		result.SetRow3(Vector4::UnitW);
		return result;
	}

	Matrix4 Matrix4::CreateTranslationMatrix(Vector3 translation)
	{
		Matrix4 translationMatrix = GetIdentity();
		translationMatrix.SetRow3(Vector4(translation, 1.0f));
		return translationMatrix;
	}

	Matrix4 Matrix4::CreateYZFlipMatrix()
	{
		Matrix4 flipped(
			Vector4::UnitX,
			Vector4::UnitZ,
			Vector4::UnitY,
			Vector4::UnitW
		);
		return flipped;
	}

	Matrix4 Matrix4::Orthographic(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearDistance, float farDistance)
	{
		Matrix4 orthoMatrix;

		float invertDistToRL = 1 / (rightPlane - leftPlane);
		float invertDistToTB = 1 / (topPlane - bottomPlane);
		float invertDistToFN = 1 / (farDistance - nearDistance);

		orthoMatrix.SetRow0(Vector4::UnitX * 2.0f * invertDistToRL);
		orthoMatrix.SetRow1(Vector4::UnitY * 2.0f * invertDistToTB);
		orthoMatrix.SetRow2(Vector4::UnitZ * -2.0f * invertDistToFN);
		orthoMatrix.SetRow3(Vector4(
			-(rightPlane + leftPlane) * invertDistToRL,
			-(topPlane + bottomPlane) * invertDistToTB,
			-(farDistance + nearDistance) * invertDistToFN,
			1.0f
		));

		return orthoMatrix;
	}

	Matrix4 Matrix4::Perspective(float fieldOfView, float aspectRatio, float nearDistance, float farDistance)
	{
		float halfFov = (fieldOfView * 0.5f) * Deg2Rad;
		float top = nearDistance * std::tan(halfFov);
		float bottom = -top;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;

		float width = right - left;
		float height = top - bottom;
		float depth = farDistance - nearDistance;

		float xScale = (2.0f * nearDistance) / (width);
		float yScale = (2.0f * nearDistance) / (height);

		float a = (right + left) / width;
		float b = (top + bottom) / height;
		float zScale = -(farDistance + nearDistance) / depth;
		float d = -(2.0f * farDistance * nearDistance) / depth;

		//Matrix4 perspectiveMatrix(
		//	Vector4::Multiply(Vector4(1,0,0,0), xScale),
		//	Vector4::Multiply(Vector4(0,1,0,0), yScale),
		//	Vector4(a, b, zScale, -1),
		//	Vector4::Multiply(Vector4(0,0,1,0), d)
		//);

		Matrix4 perspectiveMatrix(
			xScale, 0, 0, 0,
			0, yScale, 0, 0,
			a, b, zScale, -1,
			0, 0, d, 0
		);

		return perspectiveMatrix;
	}

	Matrix4 Matrix4::CreateScreenSpaceMatrix(Vector2i halfSize)
	{
		Matrix4 clipMat(
			Vector4(halfSize.x, 0, 0, halfSize.x),
			Vector4(0, -halfSize.y, 0, halfSize.y),
			Vector4::UnitZ,
			Vector4::UnitW
		);

		return clipMat;
	}

	Matrix4 Matrix4::CreateFrustumMatrix(float left, float right, float bottom, float top, float nearDepth, float farDepth)
	{
		float invertRL = 1.0f / (right - left); // refactor
		float invertTB = 1.0f / (top - bottom);
		float invertFN = 1.0f / (farDepth - nearDepth);

		Matrix4 frustumMatrix(
			Vector4::UnitX * 2.0f * nearDepth * invertRL,
			Vector4::UnitY * 2.0f * nearDepth * invertTB,
			Vector4((right + left) * invertRL, (top + bottom) * invertTB, -(farDepth + nearDepth) * invertFN, -1.0f),
			Vector4::UnitZ * -2.0f * farDepth * nearDepth * invertFN
		);

		return frustumMatrix;
	}

	Matrix4 Matrix4::LookAt(Vector3 eye, Vector3 target, Vector3 up)
	{
		Vector3 z = (eye - target).Normalised();
		Vector3 x = Vector3::Cross(up, z).Normalised();
		Vector3 y = Vector3::Cross(z, x).Normalised();

		Matrix4 rot = Matrix4(
			Vector4(x.x, y.x, z.x, 0.0f),
			Vector4(x.y, y.y, z.y, 0.0f),
			Vector4(x.z, y.z, z.z, 0.0f),
			Vector4::UnitW
		);

		Matrix4 trans = CreateTranslationMatrix(Vector3(eye.x, eye.y, eye.z));

		return Matrix4::Multiply(trans, rot);
	}

	/// Get value in the matrix given the specified index.
	float Matrix4::GetValue(int index) const
	{
		return (&m00)[index];
	}

	/// Set a value in the matrix given the specified index.
	void Matrix4::SetValue(int index, float value)
	{
		(&m00)[index] = value;
	}

//}


Matrix4 operator *(Matrix4 left, Matrix4 right) { return Matrix4::Multiply(left, right); }
Vector4 operator *(Matrix4 left, Vector4 right) { return Matrix4::TransformVector(left, right); }
Vector3 operator *(Matrix4 left, Vector3 right) { return Matrix4::TransformVector(left, right); }
Vector2 operator *(Matrix4 left, Vector2 right) { return Matrix4::TransformVector(left, right); }

Matrix4 operator *=(Matrix4 left, Matrix4 right)
{
	Matrix4 result;
	result = Matrix4::Multiply(left, right);
	return result;
}