#ifndef COLOURRGBA_H
#define COLOURRGBA_H

#include "Vector4.h"
#include "Vector3.h"
#include "Colour.h"

#ifndef BYTE_T
#include "Byte.h"
#endif

//namespace ath
//{

	class ColourRGBA;

	class ColourRGBA
	{
	public:
		float r;
		float g;
		float b;
		float a;

		ColourRGBA();
		ColourRGBA(Vector3 v);
		ColourRGBA(Vector4 v);
		ColourRGBA(byte alpha, byte red, byte green, byte blue);
		ColourRGBA(float alpha, float red, float green, float blue);

		Vector3 GetHSV();
		void FromHSV(Vector3 colour);
		byte* ToBytes();
		Vector4 ToVec4();
		byte* ToBGRA() const;

		static ColourRGBA FromBytes(byte a, byte r, byte g, byte b);
		static ColourRGBA FromFloats(float a, float r, float g, float b);
		static Vector3 ConvertToHSV(ColourRGBA colour);
		static ColourRGBA ConvertFromHSV(Vector3 hsv);
		static ColourRGBA Divide(ColourRGBA c, float denominator);
		static ColourRGBA Scale(ColourRGBA c, float scalar);
		static ColourRGBA Add(ColourRGBA left, float right);
		static ColourRGBA Subtract(ColourRGBA left, float right);

		// Operators

		/// Get a colour component given the specified index.
		float operator () (int i) const
		{
			return (&r)[i];
		}

		//operator Vector3()
		//{
		//	Vector3 result(r, g, b);
		//	return result;
		//}

		//operator Vector4()
		//{
		//	Vector4 result(r, g, b, a);
		//	return result;
		//}

		ColourRGBA operator *(float scalar)
		{
			ColourRGBA c = *this;
			return Scale(c, scalar);
		}

		ColourRGBA operator /(float denominator)
		{
			ColourRGBA c = *this;
			return Divide(c, denominator);
		}

        ColourRGBA operator +(float scalar)
		{
			ColourRGBA c = *this;
			return Add(c, scalar);
		}

        ColourRGBA operator -(float scalar)
		{
			ColourRGBA c = *this;
			return Subtract(c, scalar);
		}
	};

//inline ColourRGBA operator +=(ColourRGBA left, Vector3 right)
//{
//    ColourRGBA result = ColourRGBA(left.r, left.g, left.b, left.a);
//	result.r += right.x;
//	result.g += right.y;
//	result.b += right.z;
//	return result;
//}

	// ColourRGBA Operators
	ColourRGBA operator +=(ColourRGBA left, Vector3 right);

//}
#endif
