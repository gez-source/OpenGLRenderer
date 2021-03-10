//#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include <cmath>
#include "float.h"
#include "MathHelpers.h"
#include "Vector2.h"
#include "Vector2i.h"
#include "Vector3.h"



//namespace ath
//{
	// Two methods to generate tangent basis vectors.

	void TriangleTangentSpace(Vector3 v0, Vector3 v1, Vector3 v2,
		Vector2 uv1, Vector2 uv2, Vector2 uv3,
		Vector3& tangent, Vector3& bitangent)
	{
		// Tangent space basis vectors.
		// Calculate the tangent and bitangent vectors for the current triangle.
		Vector3 edge1 = v1 - v0;
		Vector3 edge2 = v2 - v0;

		Vector2 deltaUV1 = uv2 - uv1;
		Vector2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = tangent.Normalise();

		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = bitangent.Normalise();
	}

	void ComputeTangentBasis(const Vector3& P0, const Vector3& P1, const Vector3& P2,
		const Vector2& UV0, const Vector2& UV1, const Vector2& UV2,
		Vector3& normal, Vector3& tangent, Vector3& binormal)
	{
		Vector3 e0 = P1 - P0;
		Vector3 e1 = P2 - P0;

		normal = Vector3::Cross(e0, e1);
		//using Eric Lengyel's approach with a few modifications
		//from Mathematics for 3D Game Programmming and Computer Graphics
		// want to be able to trasform a vector in Object Space to Tangent Space
		// such that the x-axis cooresponds to the 's' direction and the
		// y-axis corresponds to the 't' direction, and the z-axis corresponds
		// to <0,0,1>, straight up out of the texture map

		//let P = v1 - v0
		Vector3 P = P1 - P0;
		//let Q = v2 - v0
		Vector3 Q = P2 - P0;
		float s1 = UV1.x - UV0.x;
		float t1 = UV1.y - UV0.y;
		float s2 = UV2.x - UV0.x;
		float t2 = UV2.y - UV0.y;


		//we need to solve the equation
		// P = s1*T + t1*B
		// Q = s2*T + t2*B
		// for T and B


		//this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
		//[px,py,pz] = [s1,t1] * [Tx,Ty,Tz]
		// qx,qy,qz     s2,t2     Bx,By,Bz

		//multiplying both sides by the inverse of the s,t matrix gives
		//[Tx,Ty,Tz] = 1/(s1t2-s2t1) *  [t2,-t1] * [px,py,pz]
		// Bx,By,Bz                      -s2,s1	    qx,qy,qz  

		//solve this for the unormalized T and B to get from tangent to object space

		float tmp = 0.0f;
		if (fabsf(s1 * t2 - s2 * t1) <= 0.0001f)
		{
			tmp = 1.0f;
		}
		else
		{
			tmp = 1.0f / (s1 * t2 - s2 * t1);
		}

		tangent.x = (t2 * P.x - t1 * Q.x);
		tangent.y = (t2 * P.y - t1 * Q.y);
		tangent.z = (t2 * P.z - t1 * Q.z);

		tangent = tangent * tmp;

		binormal.x = (s1 * Q.x - s2 * P.x);
		binormal.y = (s1 * Q.y - s2 * P.y);
		binormal.z = (s1 * Q.z - s2 * P.z);

		binormal = binormal * tmp;

		normal = normal.Normalise();
		tangent = tangent.Normalise();
		binormal = binormal.Normalise();
	}

	float cube_root(float value)
	{
		return std::pow(value, 1.0f / 3.0f);
	}

	Vector3 ScreenSpaceToOrthographic(Vector2i screen, Vector2 screenSize, Vector2 objSize)
	{
		// Orthographic matrix must be configured like this for this conversion to work in the same coordinate space.
		// quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);
		float sizeX = (objSize.x / screenSize.x);
		float sizeY = (objSize.y / screenSize.y);
		float halfX = sizeX * 0.25f;
		float halfY = sizeY * 0.25f;

		Vector2 normalised = Vector2((screen.x / screenSize.x), (screen.y / screenSize.y));
		Vector3 orthoPosition = Vector3(((normalised.x - halfX) * 2.0f) - 1.0f, ((1.0f - normalised.y - halfY) * 2.0f) - 1.0f, -1.0f);

		return orthoPosition;
	}

	Vector3 ScreenSpaceToOrthographic(Vector2 screenNormalised, Vector2 screenSize, Vector2 objSize)
	{
		// TODO: BUGGY: Scale of objects are erroneous 
		// Orthographic matrix must be configured like this for this conversion to work in the same coordinate space.
		// quadProj = Matrix4::Orthographic(-1, 1, -1, 1, -1.0f, 1.0f);
		float sizeX = (objSize.x / screenSize.x);
		float sizeY = (objSize.y / screenSize.y);
		float halfX = sizeX * 0.25f;
		float halfY = sizeY * 0.25f;

		// Coordinates are bound to the top left corner.
		//TODO: enable referencing from other corners
		Vector3 orthoPosition = Vector3(((screenNormalised.x + halfX) * 2.0f) - 1.0f, ((1.0f - screenNormalised.y - halfY) * 2.0f) - 1.0f, -1.0f);
		//Vector3 orthoPosition = Vector3(((screenNormalised.x) * 2.0f) - 1.0f, ((1.0f - screenNormalised.y) * 2.0f) - 1.0f, -1.0f);

		return orthoPosition;
	}

	float ClampF(float value, float min, float max)
	{
		return std::max(std::min(value, max), min);
	}

	Vector2 Clamp(Vector2 uv)
	{
		Vector2 clamped(uv.x, uv.y);

		clamped.x = std::max(std::min(clamped.x, 1.0f), 0.0f);
		clamped.y = std::max(std::min(clamped.y, 1.0f), 0.0f);

		return clamped;
	}

	Vector2 Wrap(Vector2 uv)
	{
		float sphereRadius = 1.0;
		//TODO: Might want to look into Toroidal mapping for UV coordinates (which should handle 2D wrapping).

		// Map 3D UV Sphere to 2D uv coordinate:
		// u = 0.5 + ATan2(z,x)/twopi
		// v = 0.5 - ASin(y) / pi

		// Inverse function of above is Mercator projection/spherical wrapping:

		// https://en.wikipedia.org/wiki/Mercator_projection
		float longitude = uv.x / sphereRadius;
		float latitude = (2.0 * std::atan(std::exp(uv.y / sphereRadius))) - PiOverTwo;

		Vector3 xyz(
			sphereRadius * cos(latitude) * cos(longitude),
			sphereRadius * cos(latitude) * sin(longitude),
			sphereRadius * sin(latitude)
		);

		return xyz.xy();
	}

	Vector2 NormaliseTexelCoord(Vector2 uv, TexelMode texelMode)
	{
		Vector2 normalised = uv;
		switch (texelMode)
		{
		case TexelMode::TexelMode_Clamp:
			normalised = Clamp(uv);
			break;
		case TexelMode::TexelMode_Wrap:
			normalised = Wrap(uv);
			break;
		}
		return normalised;
	}

	Vector2i ClampImageSpace(Vector2i texelCoord, Vector2i rectBounds)
	{
		Vector2i clamped(
			(int)std::max(std::min((double)texelCoord.x, rectBounds.x - 1.0), 0.0),
			(int)std::max(std::min((double)texelCoord.y, rectBounds.y - 1.0), 0.0)
		);
		return clamped;
	}

// MathHelpers.cpp
    ColourRGBA MathHelpers::ToColour(Vector4 v)
    {
        ColourRGBA result(v.w, v.x, v.y, v.z);
        return result;
    }

    Vector4 MathHelpers::ToVector(ColourRGBA c)
    {
        Vector4 result(c.r, c.g, c.b, c.a);
        return result;
    }
	Vector3 MathHelpers::ConvertColourToNormalSpace(Vector3 colour)
	{
		// Convert from 0 and 1 to -1, 1.
		Vector3 normalCoord((2 * colour.x) - 1, (2 * colour.y) - 1, (2 * colour.z) - 1);

		return normalCoord;
	}
	Vector3 MathHelpers::ConvertNormalToColourSpace(Vector3 normal)
	{
		//if (normal.x < 0) normal.x *= -1;  // normals must be in 0 - 1 range not (-1 to 1)
		//if (normal.y < 0) normal.y *= -1;

		Vector3 N = Vector3(normal.x, normal.y, normal.z);
		N += Vector3::One;
		N = N.Normalised();
		return N;
	}

	Vector3 MathHelpers::RandomPointInCube()
	{
		return GetRandomPointInCube();
	}

	Vector3 MathHelpers::RandomPointInUnitSphere()
	{
		return GetRandomPointInSphere();
	}

	Vector3 MathHelpers::RandomPointInFrontOfPlane()
	{
		return GetRandomPointInFrontOfPlane();
	}

	Vector3 MathHelpers::GetRandomPointInFrontOfPlane()
	{
		return GetRandomPointInFrontOfPlane(0.00001f);
	}

	Vector3 MathHelpers::GetRandomPointInFrontOfPlane(float zOffset)
	{
		Vector3 point = Vector3(0,0,0);
		point.x = (2 * RandomFloat() - 0.5f);
		point.y = (2 * RandomFloat() - 0.5f);
		point.z = (RandomFloat() + zOffset);
		return point;
	}

	Vector3 MathHelpers::GetRandomPointInCube()
	{
		// return RandomPointInUnitSphere;

		return Vector3(NormaliseBetweenNegPve(RandomFloat()),
			NormaliseBetweenNegPve(RandomFloat()),
			NormaliseBetweenNegPve(RandomFloat()));
	}

	int MathHelpers::Clamp(int value, int min, int max)
	{
		int v = value;
		if (value < min) v = min;
		if (value > max) v = max;
		return v;
	}

	float MathHelpers::Clamp(float value, float min, float max)
	{
		float v = value;
		if (value < min) v = min;
		if (value > max) v = max;
		return v;
	}

	//TODO: kinda belongs in each vector class
	Vector3 MathHelpers::ClampVector(Vector3 v, float min, float max)
	{
		return Vector3(Clamp(v.x, min, max), Clamp(v.y, min, max), Clamp(v.z, min, max));
	}
	Vector3 MathHelpers::ClampVector(Vector3 v, float min, Vector3 max)
	{
		return Vector3(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y), Clamp(v.z, min, max.z));
	}
	Vector2 MathHelpers::ClampVector(Vector2 v, float min, float max)
	{
		return Vector2(Clamp(v.x, min, max), Clamp(v.y, min, max));
	}
	Vector2 MathHelpers::ClampVector(Vector2 v, float min, Vector2 max)
	{
		return Vector2(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y));
	}
	Vector2i MathHelpers::ClampVector(Vector2i v, int min, int max)
	{
		return Vector2i(Clamp(v.x, min, max), Clamp(v.y, min, max));
	}
	Vector2i MathHelpers::ClampVector(Vector2i v, int min, Vector2i max)
	{
		return Vector2i(Clamp(v.x, min, max.x), Clamp(v.y, min, max.y));
	}
	//TODO: kinda belongs in each vector class
	float MathHelpers::Lerp(float left, float right, float ratio)
	{
		float result;
		result = left + ((right - left) * ratio);
		return result;
	}

	Vector2 MathHelpers::Lerp(Vector2 left, Vector2 right, float ratio)
	{
		Vector2 result;
		result.x = left.x + ((right.x - left.x) * ratio);
		result.y = left.y + ((right.y - left.y) * ratio);
		return result;
	}

	Vector3 MathHelpers::Lerp(Vector3 left, Vector3 right, float ratio)
	{
		Vector3 result;
		result.x = left.x + ((right.x - left.x) * ratio);
		result.y = left.y + ((right.y - left.y) * ratio);
		result.z = left.z + ((right.z - left.z) * ratio);
		return result;
	}

	//Vector4 MathHelpers::Lerp(Vector4 left, Vector4 right, float ratio)
	//{
	//	Vector4 result;
	//	result.x = left.x + ((right.x - left.x) * ratio);
	//	result.y = left.y + ((right.y - left.y) * ratio);
	//	result.z = left.z + ((right.z - left.z) * ratio);
	//	result.w = left.w + ((right.w - left.w) * ratio);
	//	return result;
	//}

	/// <summary>
	/// Normalise value between -1 and 1.
	/// </summary>
	float MathHelpers::NormaliseBetweenNegPve(float value)
	{
		return (value - 0.5f) * 2;
	}

	float MathHelpers::RandomFloat()
	{
		return ((float)rand()) / (float)RAND_MAX;
	}

	float MathHelpers::Range(float min, float max) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	Vector3 MathHelpers::GetRandomPointInSphere(float radius)
	{
		float v = RandomFloat();

		float theta = RandomFloat() * TwoPi;
		float phi = acos(2 * v - 1);

		float x = (radius * cos(theta) * sin(phi));
		float y = (radius * sin(theta) * sin(phi));
		float z = (radius * cos(phi));

		return Vector3(x, y, z);
	}

	Vector3 MathHelpers::GetPointInSphere(float radius, float theta, float phiValue)
	{
		float phi = acos(2 * phiValue - 1);

		float x = (radius * cos(theta) * sin(phi));
		float y = (radius * sin(theta) * sin(phi));
		float z = (radius * cos(phi));

		return Vector3(x, y, z);

	}

//}
