#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include "ColourRGBA.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2i.h"
#include "Vector2.h"

//namespace ath
//{
	enum TexelMode
	{
		TexelMode_Clamp,
		TexelMode_Wrap
	};

	void TriangleTangentSpace(Vector3 v0, Vector3 v1, Vector3 v2,
		Vector2 uv1, Vector2 uv2, Vector2 uv3,
		Vector3& tangent, Vector3& bitangent);

	void ComputeTangentBasis(const Vector3& P0, const Vector3& P1, const Vector3& P2,
		const Vector2& UV0, const Vector2& UV1, const Vector2& UV2,
		Vector3& normal, Vector3& tangent, Vector3& binormal);

	float cube_root(float value);

	Vector3 ScreenSpaceToOrthographic(Vector2i screen, Vector2 screenSize, Vector2 objSize);
	Vector3 ScreenSpaceToOrthographic(Vector2 screenNormalised, Vector2 screenSize, Vector2 objSize);

	float ClampF(float value, float min, float max);

	Vector2 Clamp(Vector2 uv);

	Vector2 Wrap(Vector2 uv);

	Vector2 NormaliseTexelCoord(Vector2 uv, TexelMode texelMode);

	Vector2i ClampImageSpace(Vector2i texelCoord, Vector2i rectBounds);


	class MathHelpers
	{
	public:
	    static ColourRGBA ToColour(Vector4 v);
        static Vector4 ToVector(ColourRGBA c);
		static Vector3 ConvertColourToNormalSpace(Vector3 colour);
		static Vector3 ConvertNormalToColourSpace(Vector3 normal);
		static Vector3 RandomPointInCube();
		static Vector3 RandomPointInUnitSphere();
		static Vector3 RandomPointInFrontOfPlane();
		static Vector3 GetRandomPointInFrontOfPlane();
		static Vector3 GetRandomPointInFrontOfPlane(float zOffset);
		static Vector3 GetRandomPointInCube();
		static int Clamp(int value, int min, int max);
		static float Clamp(float value, float min, float max);

		//TODO: kinda belongs in each vector class
		static Vector3 ClampVector(Vector3 v, float min, float max);
		static Vector3 ClampVector(Vector3 v, float min, Vector3 max);
		static Vector2 ClampVector(Vector2 v, float min, float max);
		static Vector2 ClampVector(Vector2 v, float min, Vector2 max);
		static Vector2i ClampVector(Vector2i v, int min, int max);
		static Vector2i ClampVector(Vector2i v, int min, Vector2i max);
		//TODO: kinda belongs in each vector class
		static float Lerp(float left, float right, float ratio);

		static Vector2 Lerp(Vector2 left, Vector2 right, float ratio);
		static Vector3 Lerp(Vector3 left, Vector3 right, float ratio);

		//static Vector4 Lerp(Vector4 left, Vector4 right, float ratio);

		/// <summary>
		/// Normalise value between -1 and 1.
		/// </summary>
		static float NormaliseBetweenNegPve(float value);

		static float RandomFloat();

		static float Range(float min, float max);

		static Vector3 GetRandomPointInSphere()
		{
			return GetRandomPointInSphere(1.0f);
		}

		static Vector3 GetRandomPointInSphere(float radius);

		static Vector3 GetPointInSphere(float radius, float theta, float phiValue);
	};



//}

#endif
