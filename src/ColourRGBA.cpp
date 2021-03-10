#include "ColourRGBA.h"

//namespace ath
//{
	ColourRGBA::ColourRGBA()
	{
		r = 0;
		g = 0;
		b = 0;
	}

	ColourRGBA::ColourRGBA(Vector3 v)
	{
		r = v.x;
		g = v.y;
		b = v.z;
	}

	ColourRGBA::ColourRGBA(Vector4 v)
	{
		r = v.x;
		g = v.y;
		b = v.z;
		a = v.w;
	}

	Vector3 ColourRGBA::GetHSV()
	{
		return Colour::ToHSV(*this);
	}

	void ColourRGBA::FromHSV(Vector3 colour)
	{
		ColourRGBA newValue = Colour::FromHSV(colour);
		a = newValue.a;
		r = newValue.r;
		g = newValue.g;
		b = newValue.b;
	}


	ColourRGBA::ColourRGBA(byte alpha, byte red, byte green, byte blue)
	{
		r = red / 255.0;
		g = green / 255.0;
		b = blue / 255.0;
		a = alpha / 255.0;
	}

	ColourRGBA::ColourRGBA(float alpha, float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	byte* ColourRGBA::ToBytes()
	{
		byte* bytes = new byte[4]{
			(byte)(a * 255),
			(byte)(r * 255),
			(byte)(g * 255),
			(byte)(b * 255)
		};

		return bytes;
	}

	Vector4 ColourRGBA::ToVec4()
	{
		return Vector4(a, r, g, b);
	}

	byte* ColourRGBA::ToBGRA() const
	{
		byte* bytes = new byte[4]{
			(byte)(b * 255),
			(byte)(g * 255),
			(byte)(r * 255),
			(byte)(a * 255)
		};

		return bytes;
	}


	ColourRGBA ColourRGBA::FromBytes(byte a, byte r, byte g, byte b)
	{
		return ColourRGBA(a / 255.0f, r / 255.0f, g / 255.0f, b / 255.0f);
	}

	ColourRGBA ColourRGBA::FromFloats(float a, float r, float g, float b)
	{
		return ColourRGBA(a, r, g, b);
	}

	Vector3 ColourRGBA::ConvertToHSV(ColourRGBA colour)
	{
		return Colour::ToHSV(colour);
	}
	ColourRGBA ColourRGBA::ConvertFromHSV(Vector3 hsv)
	{
		return Colour::FromHSV(hsv);
	}

	ColourRGBA ColourRGBA::Divide(ColourRGBA c, float denominator)
	{
		return ColourRGBA(c.a, c.r / denominator, c.g / denominator, c.b / denominator);
	}

	ColourRGBA ColourRGBA::Scale(ColourRGBA c, float scalar)
	{
		return ColourRGBA(c.a, c.r * scalar, c.g * scalar, c.b * scalar);
	}

    ColourRGBA ColourRGBA::Add(ColourRGBA left, float right)
    {
        return ColourRGBA(left.a, left.r + right, left.g * right, left.b * right);
    }

    ColourRGBA ColourRGBA::Subtract(ColourRGBA left, float right)
    {
        return ColourRGBA(left.a, left.r - right, left.g - right, left.b - right);
    }
//}

// ColourRGBA Operators
ColourRGBA operator +=(ColourRGBA left, Vector3 right)
{
	ColourRGBA result = ColourRGBA(left.r, left.g, left.b, left.a);
	result.r += right.x;
	result.g += right.y;
	result.b += right.z;
	return result;
}
