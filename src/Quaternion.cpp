#include "Quaternion.h"
#include <algorithm>
#include <cmath>

//namespace ath
//{

	// Define and Initialise static members of class Quaternion.
	//Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1);

    Quaternion Quaternion::GetIdentity()
    {
        return Quaternion(0, 0, 0, 1);
    }

	Vector2 Quaternion::xy()
	{
		Vector2 result(x, y);
		return result;
	}

	void Quaternion::xy(Vector2 value)
	{
		x = value.x;
		y = value.y;
	}

	Vector3 Quaternion::xyz()
	{
		return Vector3(x, y, z);
	}

	void Quaternion::xyz(Vector3 value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
	}

	Vector4 Quaternion::xyzw()
	{
		return Vector4(x, y, z, w);
	}

	void Quaternion::xyzw(Vector4 value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;
	}

	Matrix4 Quaternion::RotationMatrix()
	{
		Matrix4 matrix;
		float xy = x * y;
		float xz = x * z;
		float xw = x * w;
		float yz = y * z;
		float yw = y * w;
		float zw = z * w;
		float xSquared = x * x;
		float ySquared = y * y;
		float zSquared = z * z;
		matrix.m00 = 1 - 2 * (ySquared + zSquared);
		matrix.m01 = 2 * (xy - zw);
		matrix.m02 = 2 * (xz + yw);
		matrix.m03 = 0;
		matrix.m10 = 2 * (xy + zw);
		matrix.m11 = 1 - 2 * (xSquared + zSquared);
		matrix.m12 = 2 * (yz - xw);
		matrix.m13 = 0;
		matrix.m20 = 2 * (xz - yw);
		matrix.m21 = 2 * (yz + xw);
		matrix.m22 = 1 - 2 * (xSquared + ySquared);
		matrix.m23 = 0;
		matrix.m30 = 0;
		matrix.m31 = 0;
		matrix.m32 = 0;
		matrix.m33 = 1;
		return matrix;
	}

	Quaternion Quaternion::FromMatrix(Matrix4 matrix) 
{
		float w, x, y, z;
		float diagonal = matrix.m00 + matrix.m11 + matrix.m22;

		if (diagonal > 0) 
		{
			float w4 = (std::sqrt(diagonal + 1.0f) * 2.0f);
			w = w4 / 4.0f;
			x = (matrix.m21 - matrix.m12) / w4;
			y = (matrix.m02 - matrix.m20) / w4;
			z = (matrix.m10 - matrix.m01) / w4;
		}
		else if ((matrix.m00 > matrix.m11) && (matrix.m00 > matrix.m22)) 
		{
			float x4 = (std::sqrt(1.0f + matrix.m00 - matrix.m11 - matrix.m22) * 2.0f);
			w = (matrix.m21 - matrix.m12) / x4;
			x = x4 / 4.0f;
			y = (matrix.m01 + matrix.m10) / x4;
			z = (matrix.m02 + matrix.m20) / x4;
		}
		else if (matrix.m11 > matrix.m22) 
		{
			float y4 = (std::sqrt(1.0f + matrix.m11 - matrix.m00 - matrix.m22) * 2.0f);
			w = (matrix.m02 - matrix.m20) / y4;
			x = (matrix.m01 + matrix.m10) / y4;
			y = y4 / 4.0f;
			z = (matrix.m12 + matrix.m21) / y4;
		}
		else 
		{
			float z4 = (std::sqrt(1.0f + matrix.m22 - matrix.m00 - matrix.m11) * 2.0f);
			w = (matrix.m10 - matrix.m01) / z4;
			x = (matrix.m02 + matrix.m20) / z4;
			y = (matrix.m12 + matrix.m21) / z4;
			z = z4 / 4.0f;
		}
		return Quaternion(x, y, z, w);
	}

	Quaternion::Quaternion() { }

	Quaternion::Quaternion(float X, float Y, float Z, float W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}

	Quaternion::Quaternion(Vector4 vector)
	{
		xyzw(vector);
	}

	Quaternion::Quaternion(Vector3 vector, float w)
	{
		xyzw(Vector4(vector, w));
	}

	Quaternion::Quaternion(Matrix4 matrix)
	{
		float scale = pow(matrix.GetDeterminant(), 1.0f / 3.0f);

		w = (std::sqrt(std::max(0.0f, scale + matrix.m00 + matrix.m11 + matrix.m22)) / 2.0f);
		x = (std::sqrt(std::max(0.0f, scale + matrix.m00 - matrix.m11 - matrix.m22)) / 2.0f);
		y = (std::sqrt(std::max(0.0f, scale - matrix.m00 + matrix.m11 - matrix.m22)) / 2.0f);
		z = (std::sqrt(std::max(0.0f, scale - matrix.m00 - matrix.m11 + matrix.m22)) / 2.0f);

		if (matrix.m12 - matrix.m12 < 0) x = -x;
		if (matrix.m02 - matrix.m20 < 0) y = -y;
		if (matrix.m10 - matrix.m01 < 0) z = -z;
	}

	/// <summary>
	/// Deconstruct quaternion into axis and angle
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void Quaternion::DeconstructQuaternion(Vector3& axis, float& angle)
	{
		Vector4 result = ToAxisAngle();
		axis = result.xyz();
		angle = result.w;
	}

	/// <summary>
	/// Deconstruct quaternion into Forward and Up vectors
	/// </summary>
	/// <param name="qOrientation"></param>
	/// <param name="newForwardVector"></param>
	/// <param name="newUpVector"></param>
	void Quaternion::DeconstructQuaternion(Vector3 newForwardVector, Vector3 newUpVector)
	{
		DeconstructQuaternion(*this, newForwardVector, newUpVector);
	}

	/// <summary>
	/// Deconstruct quaternion into Forward and Up vectors
	/// </summary>
	/// <param name="qOrientation"></param>
	/// <param name="newForwardVector"></param>
	/// <param name="newUpVector"></param>
	void Quaternion::DeconstructQuaternion(Quaternion qOrientation, Vector3 newForwardVector, Vector3 newUpVector)
	{
		// How to update local Forward and Up vectors given current Quaternion rotation:
		Vector3 worldUp = Vector3::Up;
		Vector3 worldBack = Vector3::Back;

		newForwardVector = Quaternion::Multiply(qOrientation, worldBack); // Negated direction since we are facing camera
		newUpVector = Quaternion::Multiply(qOrientation, worldUp);
	}

	Quaternion Quaternion::LookRotation(Vector3 lookAt, Vector3 up)
	{
		Vector3 forward = lookAt.Normalised();
		Vector3 right = Vector3::Cross(up, forward);

		Quaternion ret;
		ret.w = sqrt(1.0f + right.x + up.y + forward.z) * 0.5f;
		float w4_recip = 1.0f / (4.0f * ret.w);
		ret.x = (forward.y - up.z) * w4_recip;
		ret.y = (right.z - forward.x) * w4_recip;
		ret.z = (up.x - right.y) * w4_recip;

		return ret;
	}

	Quaternion Quaternion::Euler(float x, float y, float z)
	{
		// Assuming the angles are in degrees.
		float c1 = cos((x * Deg2Rad) / 2);
		float s1 = sin((x * Deg2Rad) / 2);
		float c2 = cos((y * Deg2Rad) / 2);
		float s2 = sin((y * Deg2Rad) / 2);
		float c3 = cos((z * Deg2Rad) / 2);
		float s3 = sin((z * Deg2Rad) / 2);
		float c1c2 = c1 * c2;
		float s1s2 = s1 * s2;
		Quaternion q;
		q.w = c1c2 * c3 - s1s2 * s3;
		q.x = c1c2 * s3 + s1s2 * c3;
		q.y = s1 * c2 * c3 + c1 * s2 * s3;
		q.z = c1 * s2 * c3 - s1 * c2 * s3;
		return q;
	}

	void Quaternion::ToAxisAngle(Vector3 axis, float angle)
	{
		Vector4 result = ToAxisAngle();
		axis = result.xyz();
		angle = result.w;
	}

	Vector4 Quaternion::ToAxisAngle()
	{
		Quaternion q = *this;

		if (abs(q.w) > 1.0f)
		{
			q.Normalise();
		}


		Vector4 result;

		result.w = 2.0f * acos(q.w);

		float scale = sqrt(1.0f - q.w * q.w);

		if (scale > 0.0001f)
		{
			result.xyz(q.xyz() / scale);
		}
		else
		{
			// This occurs when the angle is zero.
			// Not a problem: just set an arbitrary normalized axis.
			result.xyz(Vector3::UnitX);
		}

		return result;
	}

	Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float ratio)
	{
		Quaternion result = Quaternion(0, 0, 0, 1);
		float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
		float blendI = 1.0f - ratio;
		if (dot < 0) 
		{
			result.w = blendI * q1.w + ratio * -q2.w;
			result.x = blendI * q1.x + ratio * -q2.x;
			result.y = blendI * q1.y + ratio * -q2.y;
			result.z = blendI * q1.z + ratio * -q2.z;
		}
		else {
			result.w = blendI * q1.w + ratio * q2.w;
			result.x = blendI * q1.x + ratio * q2.x;
			result.y = blendI * q1.y + ratio * q2.y;
			result.z = blendI * q1.z + ratio * q2.z;
		}
		result.Normalise();
		return result;
	}

	Quaternion Quaternion::Conjugate()
	{
		Quaternion result(-xyz(), w);
		return result;
	}

	Quaternion Quaternion::Normalised()
	{
		Quaternion norm(xyzw());
		float scale = 1.0f / norm.Magnitude();
		Vector4 xyzw = norm.xyzw();
		xyzw *= scale;
		norm.xyzw(xyzw);
		return norm;
	}

	void Quaternion::Normalise()
	{
		float scale = 1.0f / Magnitude();
		Vector4 q = xyzw();
		q *= scale;
		xyzw(q);
	}

	float Quaternion::Magnitude()
	{
		return xyzw().Magnitude();
	}

	float Quaternion::MagnitudeSquared()
	{
		return xyzw().MagnitudeSquared();
	}

	Quaternion Quaternion::Add(Quaternion left, Quaternion right)
	{
		Quaternion result(left.xyzw() + right.xyzw());
		return result;
	}

	Quaternion Quaternion::Add(Quaternion left, float right)
	{
		Vector4 rhs = Vector4::One * right;
		Quaternion result(Multiply(left, rhs));
		return result;
	}

	Quaternion Quaternion::Subtract(Quaternion left, Quaternion right)
	{
		Quaternion result(left.xyzw() - right.xyzw());
		return result;
	}

	Quaternion Quaternion::Subtract(Quaternion left, float right)
	{
		Quaternion result(left.xyzw() - right);
		return result;
	}

	Vector3 Quaternion::Multiply(Quaternion left, Vector3 right)
	{
		Vector4 rhs(right, 0);
		Vector3 result = Multiply(left, rhs);
		return result;
	}

	Vector4 Quaternion::Multiply(Quaternion left, Vector4 right)
	{
		// Only works if input is a unit quaternion.
		Quaternion norm = left.Normalised();
		//right.w = 1.0f;
		Quaternion p = Quaternion(right);
		Quaternion c = norm.Conjugate();
		Vector4 result = Quaternion::Multiply(Quaternion::Multiply(norm, p), c).xyzw();
		return result;
	}

	Quaternion Quaternion::Multiply(Quaternion left, Quaternion right)
	{
		Quaternion result(
			(left.xyz() * right.w) +
			(right.xyz() * left.w) +
			Vector3::Cross(left.xyz(), right.xyz()),
			(left.w * right.w) - Vector3::Dot(left.xyz(), right.xyz())
		);
		return result;
	}

	Quaternion Quaternion::Scale(Quaternion quaternion, float scale)
	{
		Quaternion result(
			quaternion.x * scale,
			quaternion.y * scale,
			quaternion.z * scale,
			quaternion.w * scale
		);
		return result;
	}

	Quaternion Quaternion::Divide(Quaternion quaternion, float divider)
	{
		Quaternion result(
			quaternion.x / divider,
			quaternion.y / divider,
			quaternion.z / divider,
			quaternion.w / divider
		);
		return result;
	}

//}

// Quaternion Operators
Quaternion operator +(Quaternion left, Quaternion right) { return Quaternion::Add(left, right); }
Quaternion operator +(Quaternion left, float right) { return Quaternion::Add(left, right); }
Quaternion operator -(Quaternion left, Quaternion right) { return Quaternion::Subtract(left, right); }
Quaternion operator -(Quaternion left, float right) { return Quaternion::Subtract(left, right); }
Quaternion operator *(Quaternion left, Quaternion right) { return Quaternion::Multiply(left, right); }
Vector4 operator *(Quaternion left, Vector4 right) { return Quaternion::Multiply(left, right); }
Vector3 operator *(Quaternion left, Vector3 right) { return Quaternion::Multiply(left, right); }
Quaternion operator *(Quaternion left, float right) { return Quaternion::Scale(left, right); }
Quaternion operator /(Quaternion left, float right) { return Quaternion::Divide(left, right); }
