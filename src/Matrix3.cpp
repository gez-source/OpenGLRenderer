#include <algorithm>
#include <iostream>
#include "Matrix3.h"
#include "assert.h"

//namespace ath
//{
	// Define and Initialise static members of class Matrix4.
	//Matrix3 Matrix3::Identity = Matrix3::GetIdentity();

	Matrix3::Matrix3()
	{
		m00 = 0;
		m01 = 0;
		m02 = 0;

		m10 = 0;
		m11 = 0;
		m12 = 0;

		m20 = 0;
		m21 = 0;
		m22 = 0;
	}

    Matrix3::Matrix3(float v00, float v01, float v02,
                     float v10, float v11, float v12,
                     float v20, float v21, float v22)
    {
		this->m00 = v00;
		this->m01 = v01;
		this->m02 = v02;

		this->m10 = v10;
		this->m11 = v11;
		this->m12 = v12;

		this->m20 = v20;
		this->m21 = v21;
		this->m22 = v22;
    }

	Matrix3::Matrix3(Vector3 row0, Vector3 row1, Vector3 row2)
	{
		m00 = row0.x;
		m01 = row0.y;
		m02 = row0.z;

		m10 = row1.x;
		m11 = row1.y;
		m12 = row1.z;

		m20 = row2.x;
		m21 = row2.y;
		m22 = row2.z;
	}

	// Matrix Rows
	void Matrix3::SetRow0(Vector3 row0)
	{
		m00 = row0.x;
		m01 = row0.y;
		m02 = row0.z;
	}

	void Matrix3::SetRow1(Vector3 row1)
	{
		m10 = row1.x;
		m11 = row1.y;
		m12 = row1.z;
	}

	void Matrix3::SetRow2(Vector3 row2)
	{
		m20 = row2.x;
		m21 = row2.y;
		m22 = row2.z;
	}

	Vector3 Matrix3::GetRow0() const
	{
		Vector3 row0(m00, m01, m02);
		return row0;
	}

	Vector3 Matrix3::GetRow1()const
	{
		Vector3 row1(m10, m11, m12);
		return row1;
	}

	Vector3 Matrix3::GetRow2()const
	{
		Vector3 row2(m20, m21, m22);
		return row2;
	}

	// Matrix Columns

	Vector3 Matrix3::GetColumn0()const
	{
		Vector3 column0(m00, m10, m20);
		return column0;
	}

	Vector3 Matrix3::GetColumn1()const
	{
		Vector3 column1(m01, m11, m21);
		return column1;
	}

	Vector3 Matrix3::GetColumn2()const
	{
		Vector3 column2(m02, m12, m22);
		return column2;
	}

	void Matrix3::SetColumn0(Vector3 column0)
	{
		m00 = column0.x;
		m10 = column0.y;
		m20 = column0.z;
	}

	void Matrix3::SetColumn1(Vector3 column1)
	{
		m01 = column1.x;
		m11 = column1.y;
		m21 = column1.z;
	}

	void Matrix3::SetColumn2(Vector3 column2)
	{
		m02 = column2.x;
		m12 = column2.y;
		m22 = column2.z;
	}

	// Matrix Columns

	Matrix3 Matrix3::GetIdentity()
	{
		Matrix3 identity(
			Vector3::UnitX,
			Vector3::UnitY,
			Vector3::UnitZ
		);
		return identity;
	}

	Matrix3 Matrix3::Transpose()
	{
		Matrix3 m = GetIdentity();

		m.SetRow0(GetColumn0());
		m.SetRow1(GetColumn1());
		m.SetRow2(GetColumn2());

		return m;
	}

    float Matrix3::GetMagnitude() const
	{
		return GetDeterminant();
	}

	float Matrix3::GetDeterminant() const
	{
        return m00 * m11 * m22
            + m01 * m12 * m20
            + m02 * m10 * m21
            - m02 * m11 * m20
            - m00 * m12 * m21
            - m01 * m10 * m22;
	}

	Matrix3 Matrix3::Inverse()
	{
        return Invert(*this);
	}

    Matrix3 Matrix3::Invert(Matrix3 m)
	{
		Matrix3 result;
		float determinant;
		int i;

		determinant = m.GetDeterminant();


		if (determinant == 0)
		{
			std::cout << ("Matrix can not be inverted.");
			assert(0);
		}
		else
		{

			determinant = 1.0f / determinant;

            result.m00 = determinant * (m.m11 * m.m22 - m.m12 * m.m21);
            result.m01 = determinant * (m.m02 * m.m21 - m.m01 * m.m22);
            result.m02 = determinant * (m.m01 * m.m12 - m.m02 * m.m11);

            result.m10 = determinant * (m.m12 * m.m20 - m.m10 * m.m22);
            result.m11 = determinant * (m.m00 * m.m22 - m.m02 * m.m20);
            result.m12 = determinant * (m.m02 * m.m10 - m.m00 * m.m12);

            result.m20 = determinant * (m.m10 * m.m21 - m.m11 * m.m20);
            result.m21 = determinant * (m.m01 * m.m20 - m.m00 * m.m21);
            result.m22 = determinant * (m.m00 * m.m11 - m.m01 * m.m10);
		}

		return result;
	}

	Vector3 Matrix3::GetDiagonal()
	{
		// [m00]  m01   m02
		//  m10  [m11]  m12
		//  m20   m21  [m22]

		return Vector3(m00, m11, m22);
	}

	float* Matrix3::ToArray()
	{
		float result[9]
		{
			m00, m01, m02,
			m10, m11, m12,
			m20, m21, m22
		};
		return result;
	}

	void Matrix3::FromArray(float* arr)
	{
		m00 = arr[0];
		m01 = arr[1];
		m02 = arr[2];

		m10 = arr[4];
		m11 = arr[5];
		m12 = arr[6];

		m20 = arr[8];
		m21 = arr[9];
		m22 = arr[10];
	}

	void Matrix3::Multiply(Matrix3 right)
	{
		Matrix3 result = GetIdentity();

		// Column 0
		result.m00 = Vector3::Dot(GetRow0(), right.GetColumn0());
		result.m01 = Vector3::Dot(GetRow0(), right.GetColumn1());
		result.m02 = Vector3::Dot(GetRow0(), right.GetColumn2());

		// Column 1
		result.m10 = Vector3::Dot(GetRow1(), right.GetColumn0());
		result.m11 = Vector3::Dot(GetRow1(), right.GetColumn1());
		result.m12 = Vector3::Dot(GetRow1(), right.GetColumn2());

		// Column 2
		result.m20 = Vector3::Dot(GetRow2(), right.GetColumn0());
		result.m21 = Vector3::Dot(GetRow2(), right.GetColumn1());
		result.m22 = Vector3::Dot(GetRow2(), right.GetColumn2());

		SetRow0(result.GetRow0());
		SetRow1(result.GetRow1());
		SetRow2(result.GetRow2());
	}

	Matrix3 Matrix3::Multiply(Matrix3 left, Matrix3 right)
	{
		Matrix3 result = GetIdentity();

		// Column 0
		result.m00 = Vector3::Dot(left.GetRow0(), right.GetColumn0());
		result.m01 = Vector3::Dot(left.GetRow0(), right.GetColumn1());
		result.m02 = Vector3::Dot(left.GetRow0(), right.GetColumn2());

		// Column 1
		result.m10 = Vector3::Dot(left.GetRow1(), right.GetColumn0());
		result.m11 = Vector3::Dot(left.GetRow1(), right.GetColumn1());
		result.m12 = Vector3::Dot(left.GetRow1(), right.GetColumn2());

		// Column 2
		result.m20 = Vector3::Dot(left.GetRow2(), right.GetColumn0());
		result.m21 = Vector3::Dot(left.GetRow2(), right.GetColumn1());
		result.m22 = Vector3::Dot(left.GetRow2(), right.GetColumn2());

		return result;
	}

	Matrix3 Matrix3::Multiply(Matrix3 left, Vector3 right)
	{
		Matrix3 result = GetIdentity();

		result.m00 = (left.m00 * right.x);
		result.m01 = (left.m00 * right.y);
		result.m02 = (left.m00 * right.z);

		result.m10 = (left.m10 * right.x);
		result.m11 = (left.m10 * right.y);
		result.m12 = (left.m10 * right.z);

		result.m20 = (left.m20 * right.x);
		result.m21 = (left.m20 * right.y);
		result.m22 = (left.m20 * right.z);

		result.m30 = (left.m30 * right.x);
		result.m31 = (left.m30 * right.y);
		result.m32 = (left.m30 * right.z);

		return result;
	}

	Vector2 Matrix3::TransformVector(Matrix3 left, Vector2 right)
	{
		Vector2 result
		(
			Vector2::Dot(right, left.GetColumn0().xy()),
			Vector2::Dot(right, left.GetColumn1().xy())
		); // Neater to do with Dot Products.
		return result;
	}

	Vector3 Matrix3::TransformVector(Matrix3 left, Vector3 right)
	{
		Vector3 result; // Neater to do with Dot Products.
		result.x = Vector3::Dot(right, left.GetColumn0());
		result.y = Vector3::Dot(right, left.GetColumn1());
		result.z = Vector3::Dot(right, left.GetColumn2());
		return result;
	}

	Vector3 Matrix3::TransformVector(Vector3 vector)
	{
        Vector3 result(
			Vector3::Dot(vector, this->GetColumn0()),
			Vector3::Dot(vector, this->GetColumn1()),
			Vector3::Dot(vector, this->GetColumn2())
		); // Neater to do with Dot Products.
		return result;
	}

	/*
	Vector3 Matrix3::TransformNormal(Vector3 normal)
	{
		Vector3 result;
		Matrix3 transform = *this;
		Matrix3 m = transform.Inverse();

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

	Vector3 Matrix3::TransformNormal(Matrix3 transform, Vector3 normal)
	{
		Vector3 result;
		Matrix3 m = static_cast<Matrix3>(transform).Inverse();

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
	*/

	/// Get value in the matrix given the specified index.
	float Matrix3::GetValue(int index) const
	{
		return (&m00)[index];
	}

	/// Set a value in the matrix given the specified index.
	void Matrix3::SetValue(int index, float value)
	{
		(&m00)[index] = value;
	}

//}
