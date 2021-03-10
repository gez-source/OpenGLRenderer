#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Triangle.h"

class Edge
{
private:
    float xStep;

public:
    float x;
    int yStart;
    int yEnd;

    Vector3 Start;
    Vector3 End;

    // Attribute value stepping
    Vector3 Position;
    Vector3 positionStep;
    Vector4 Colour;
    Vector4 colourStep;
    Vector3 Normal;
    Vector3 normalStep;
    Vector2 TexCoord;
    Vector2 texCoordStep;

    Triangle triangle;

    Edge(Triangle t, Vector3 start, Vector3 end, int minYVertIndex)
    {
        this->triangle = t;

        Start = start;
        End = end;

        yStart = (int)std::ceil(start.y);
        yEnd = (int)std::ceil(end.y);

        float yDist = end.y - start.y;
        float xDist = end.x - start.x;

        float yPrestep = yStart - start.y;
        xStep = (float)xDist / (float)yDist;
        x = start.x + yPrestep * xStep;

        float xPrestep = x - start.x;

        // Interpolate from vertex attributes nearest to scanline.

        Position = Vector3::Add(Vector3::Add(GetPosition(minYVertIndex) ,
                 (triangle.positionYStep * yPrestep)) , (triangle.positionXStep * xPrestep));

        positionStep = Vector3::Add(triangle.positionYStep , (triangle.positionXStep * xStep));


        if (triangle.HasVertexColours())
        {

            Colour = Vector4::Add(Vector4::Add(GetColor(minYVertIndex),
            Vector4::Multiply(triangle.colourYStep, yPrestep)),
            Vector4::Multiply(triangle.colourXStep, xPrestep));

            colourStep = Vector4::Add(triangle.colourYStep, Vector4::Multiply(triangle.colourXStep, xStep));


        }

        if(triangle.HasVertexNormals())
        {
            Normal = GetNormal(minYVertIndex) +
                (triangle.normalYStep * yPrestep) + (triangle.normalXStep * xPrestep);

            normalStep = triangle.normalYStep + (triangle.normalXStep * xStep);
        }

        if (triangle.HasVertexTexCoords())
        {
            TexCoord = GetTexCoord(minYVertIndex) +
                     (triangle.texCoordYStep * yPrestep) + (triangle.texCoordXStep * xPrestep);

            texCoordStep = triangle.texCoordYStep + (triangle.texCoordXStep * xStep);
        }
    }

    Vector3 GetPosition(int vertIndex)
    {
        Vector3 m0 = triangle.vertex00;
        Vector3 m1 = triangle.vertex01;
        Vector3 m2 = triangle.vertex10;

        if (vertIndex == 0)
            return m0;
        if (vertIndex == 1)
            return m1;
        return m2;
    }


    Vector4 GetColor(int vertIndex)
    {
        Vector4 m0 = triangle.colour00.get();
        Vector4 m1 = triangle.colour01.get();
        Vector4 m2 = triangle.colour10.get();

        if (vertIndex == 0)
            return m0;
        if (vertIndex == 1)
            return m1;
        return m2;
    }

    Vector3 GetNormal(int vertIndex)
    {
        Vector3 m0 = triangle.normal00.get();
        Vector3 m1 = triangle.normal01.get();
        Vector3 m2 = triangle.normal10.get();

        if (vertIndex == 0)
            return m0;
        if (vertIndex == 1)
            return m1;
        return m2;
    }

    Vector2 GetTexCoord(int vertIndex)
    {
        Vector2 m0 = triangle.texCoord00.get();
        Vector2 m1 = triangle.texCoord01.get();
        Vector2 m2 = triangle.texCoord10.get();

        if (vertIndex == 0)
            return m0;
        if (vertIndex == 1)
            return m1;
        return m2;
    }

    void Step()
    {
        x += xStep;

        Position = Vector3::Add(Position, positionStep);

        if (triangle.HasVertexColours())
        {
            Colour = Vector4::Add(Colour, colourStep);
        }

        if (triangle.HasVertexNormals())
        {
            Normal = Vector3::Add(Normal, normalStep);
        }

        if (triangle.HasVertexTexCoords())
        {
            TexCoord = Vector2::Add(TexCoord, texCoordStep);
        }
    }
};

