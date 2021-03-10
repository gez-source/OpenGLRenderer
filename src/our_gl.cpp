#include <cmath>
#include <limits>
#include <climits>
#include <cstdlib>
#include "our_gl.h"
#include "Framebuffer.h"
#include "Vector3.h"
#include "Vector2i.h"

Matrix4 ToMatrix4(Matrix mat)
{
    Matrix m = mat;
    //m = mat.transpose();
    Matrix4 result;
    result.m00 = m[0][0];
    result.m01 = m[0][1];
    result.m02 = m[0][2];
    result.m03 = m[0][3];

    result.m10 = m[1][0];
    result.m11 = m[1][1];
    result.m12 = m[1][2];
    result.m13 = m[1][3];

    result.m20 = m[2][0];
    result.m21 = m[2][1];
    result.m22 = m[2][2];
    result.m23 = m[2][3];

    result.m30 = m[3][0];
    result.m31 = m[3][1];
    result.m32 = m[3][2];
    result.m33 = m[3][3];

    return result;
}

Matrix ToMatrix(Matrix4 m)
{
    Matrix result;
    result[0][0] = m.m00;
    result[0][1] = m.m01;
    result[0][2] = m.m02;
    result[0][3] = m.m03;

    result[1][0] = m.m10;
    result[1][1] = m.m11;
    result[1][2] = m.m12;
    result[1][3] = m.m13;

    result[2][0] = m.m20;
    result[2][1] = m.m21;
    result[2][2] = m.m22;
    result[2][3] = m.m23;

    result[3][0] = m.m30;
    result[3][1] = m.m31;
    result[3][2] = m.m32;
    result[3][3] = m.m33;
    return result;
}


float orient2d(const Vector3& a, const Vector3& b, const Vector2i& c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

float min3(float x, float y, float z)
{
    float result = x;

    if(y < result)
    {
        result = y;
    }

    if(z < result)
    {
        result = z;
    }

    return result;
}

float max3(float x, float y, float z)
{
    float result = x;

    if(y > result)
    {
        result = y;
    }

    if(z > result)
    {
        result = z;
    }

    return result;
}

float min(float x, float y)
{
    float result = x;
    if(y < result)
    {
        result = y;
    }
    return result;
}

float max(float x, float y)
{
    float result = x;
    if(y > result)
    {
        result = y;
    }
    return result;
}

void EFLA_Line_Modified(Vector2i begin, Vector2i end, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    // EFLA Line Drawing Algorithm (faster than Bresenhams)
    // Ported to C++ from the following: http://www.simppa.fi/blog/extremely-fast-line-algorithm-as3-optimized/
    // Modified for fragment shader.
    int x = begin.x;
    int y = begin.y;
    int x2 = end.x;
    int y2 = end.y;
    float multDiff;
    int inc;
    int i;
    bool yLonger;
    int shortLen = y2 - y;
    int longLen = x2 - x;

    if((shortLen ^ (shortLen >> 31)) - (shortLen >> 31) > (longLen ^ (longLen >> 31)) - (longLen >> 31))
    {
        shortLen ^= longLen;
        longLen ^= shortLen;
        shortLen ^= longLen;

        yLonger = true;
    }
    else
    {
        yLonger = false;
    }

    inc = longLen < 0 ? -1 : 1;

    multDiff = longLen == 0 ? (float)shortLen : (float)shortLen / (float)longLen;

    ColourRGBA fragColour;
    Vector3 bar;

    if (yLonger)
    {
        for (i = 0; i != longLen; i += inc)
        {
            //bar = Vector3(0,0,0);
            //bool discard = shader.fragment(model, bar, fragColour);
            fragColour = Colour::green;
            //if(!discard)
            {
                framebuffer->SetPixel(x + i * multDiff, y + i, fragColour);
            }
        }
    }
    else
    {
        for (i = 0; i != longLen; i += inc)
        {
        // TODO: Somehow implement barycentric coordinates
            //bar = Vector3(0,0,0);
            //bool discard = shader.fragment(model, bar, fragColour);
            //Vector2 uv = Vector2(i/(float)longLen, i/(float)longLen);
            //fragColour = model->diffuse(uv);
            fragColour = Colour::red;
            //if(!discard)
            {
                framebuffer->SetPixel(x + i, y + i * multDiff, fragColour);
            }
        }
    }

}

void Bresenhams_Line_Modified(Vector2i begin, Vector2i end, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    // Bresenham Line Algorithm
    int x, y, deltaX, deltaY, numerator;
    int longest;
    int shortest;
    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    int i;
    ColourRGBA fragColour;

    x = begin.x;
    y = begin.y;
    deltaX = (end.x - begin.x);
    deltaY = (end.y - begin.y);

    if (deltaX < 0)
    {
        dx1 = -1;
    }
    else if (deltaX > 0)
    {
        dx1 = 1;
    }

    if (deltaY < 0)
    {
        dy1 = -1;
    }
    else if (deltaY > 0)
    {
        dy1 = 1;
    }

    if (deltaX < 0)
    {
        dx2 = -1;
    }
    else if (deltaX > 0)
    {
        dx2 = 1;
    }

    if (deltaX <= INT_MIN || deltaY <= INT_MIN)
    {
        return;
    }

    longest = std::abs(deltaX);
    shortest = std::abs(deltaY);

    if (longest <= shortest)
    {
        longest = std::abs(deltaY);
        shortest = std::abs(deltaX);

        if (deltaY < 0)
        {
            dy2 = -1;
        }
        else if (deltaY > 0)
        {
            dy2 = 1;
        }
        dx2 = 0;
    }

    numerator = longest >> 1;

    for (i = 0; i <= longest; i++)
    {
        fragColour = Colour::red;
        framebuffer->SetPixel(Vector2i(x, y), fragColour);

        numerator += shortest;

        if (!(numerator < longest))
        {
            numerator -= longest;

            x += dx1;
            y += dy1;
        }
        else
        {
            x += dx2;
            y += dy2;
        }
    }

    return;
}

void fillBottomFlatTriangle(Vector2i v1, Vector2i v2, Vector2i v3, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    float invslope1 = ((float)v2.x - (float)v1.x) / ((float)v2.y - (float)v1.y);
    float invslope2 = ((float)v3.x - (float)v1.x) / ((float)v3.y - (float)v1.y);

    float curx1 = v1.x;
    float curx2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
    {
        Bresenhams_Line_Modified(Vector2i((int)curx1, scanlineY), Vector2i((int)curx2, scanlineY), model, shader, framebuffer);

        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void fillTopFlatTriangle(Vector2i v1, Vector2i v2, Vector2i v3, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    float invslope1 = ((float)v3.x - (float)v1.x) / ((float)v3.y - (float)v1.y);
    float invslope2 = ((float)v3.x - (float)v2.x) / ((float)v3.y - (float)v2.y);

    float curx1 = v3.x;
    float curx2 = v3.x;
    float ratioY;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        Bresenhams_Line_Modified(Vector2i((int)curx1, scanlineY), Vector2i((int)curx2, scanlineY), model, shader, framebuffer);

        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

Triangle sortVerticesAscendingByY(Triangle t)
{
    Vector3     m;
    Vector3 v1 = t.vertex00;
    Vector3 v2 = t.vertex01;
    Vector3 v3 = t.vertex10;
    if (v1.y > v2.y)
    {
        m = v1;
        v1 = v2;
        v2 = m;
    }

    if (v1.y > v3.y)
    {
        m = v1;
        v1 = v3;
        v3 = m;
    }

    if (v2.y > v3.y)
    {
        m = v2;
        v2 = v3;
        v3 = m;
    }

    t.vertex00 = v1;
    t.vertex01 = v2;
    t.vertex10 = v3;
    return t;
}



void drawTriangle3(Vector3 eyeWorldPos, Triangle t, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    t = sortVerticesAscendingByY(t);

    Vector2i v1 = t.vertex00.xyi();
    Vector2i v2 = t.vertex01.xyi();
    Vector2i v3 = t.vertex10.xyi();

    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if (v2.y == v3.y)
    {
        fillBottomFlatTriangle(v1, v2, v3, model, shader, framebuffer);
    }
    /* check for trivial case of top-flat triangle */
    else if (v1.y == v2.y)
    {
        fillTopFlatTriangle(v1, v2, v3, model, shader, framebuffer);
    }
    else
    {
        float a = (float)(v2.y - v1.y);
        float b = (float)(v3.y - v1.y);

        /* general case - split the triangle in a topflat and bottom-flat one */
        Vector2i v4 = Vector2i(
            (int)(v1.x + (a / b) * (v3.x - v1.x)),
            v2.y
        );

        fillBottomFlatTriangle(v1, v2, v4, model, shader, framebuffer);
        fillTopFlatTriangle(v2, v4, v3, model, shader, framebuffer);
    }
}

void drawTriangle(Vector3 eyeWorldPos, Triangle t, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    Vector3 v0 = t.vertex00;
    Vector3 v1 = t.vertex01;
    Vector3 v2 = t.vertex10;

    // Compute triangle bounding box
    float minX = min3(v0.x, v1.x, v2.x);
    float minY = min3(v0.y, v1.y, v2.y);
    float maxX = max3(v0.x, v1.x, v2.x);
    float maxY = max3(v0.y, v1.y, v2.y);

    // Clip against screen bounds
    minX = max(minX, 0);
    minY = max(minY, 0);
    maxX = min(maxX, width - 1);
    maxY = min(maxY, height - 1);

    // Rasterize
    Vector2i p;
    ColourRGBA fragColour;
    float ratioX;
    float ratioY;
    Vector2 uv;
    Vector3 uvw;
    Vector3 bary;
    double z;

    for (p.y = minY; p.y <= maxY; p.y++)
    {
        for (p.x = minX; p.x <= maxX; p.x++)
        {
            // Determine barycentric coordinates
            float w0 = orient2d(v1, v2, p);
            float w1 = orient2d(v2, v0, p);
            float w2 = orient2d(v0, v1, p);

            // If p is on or inside all edges, render pixel.
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                ratioX = ((p.x - minX) / (float)(maxX - minX));
                ratioY = ((p.y - minY) / (float)(maxY - minY));
                uv = Vector2(ratioX, ratioY);

                uvw = t.TriangleLerp(uv);
                bary = Vector3(w0, w1, w2);
                z = (double)uvw.z;
                Vec3f b = Vec3f(w0, w1, w2);

                if(framebuffer->DepthTest(p, z))
                {
                    bool discard = shader.fragment(eyeWorldPos, model, bary, fragColour);
                    if (!discard)
                    {
                        fragColour = Colour::red;
                        framebuffer->SetPixel(p, fragColour);
                        framebuffer->SetDepth(p, z);
                    }
                }
            }
        }
    }
}


Vector3 barycentric(Vector2 A, Vector2 B, Vector2 C, Vector2i P)
{
    Vector3 s[2];
    for (int i=2; i--; )
    {
        s[i].x = C[i]-A[i];
        s[i].y = B[i]-A[i];
        s[i].z = A[i]-P[i];
    }
    Vector3 u = Vector3::Cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
    {
        return Vector3(1.f-(u.x + u.y) / u.z, u.y/u.z, u.x/u.z);
    }
    return Vector3(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}

void triangle(Vector3 eyeWorldPos, mat<4,3,float> &clipc, Model* model, Shader &shader, Framebuffer* framebuffer)
{
    mat<3,4,float> pts  = (ToMatrix(shader.viewPort) * clipc).transpose(); // transposed to ease access to each of the points
    mat<3,2,float> pts2;
    for (int i=0; i<3; i++)
    {
        pts2[i] = proj<2>(pts[i] / pts[i][3]);
    }

    Vector2 bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vector2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vector2 clamp(width-1, height-1);
    for (int i=0; i<3; i++)
    {
        //for (int j=0; j<2; j++)
        //{
        //    bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts2[i][j]));
        //    bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts2[i][j]));
        //}

        // Equivalent code
        bboxmin.x = std::max(0.f,      std::min(bboxmin.x, pts2[i][0]));
        bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, pts2[i][0]));

        bboxmin.y = std::max(0.f,      std::min(bboxmin.y, pts2[i][1]));
        bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, pts2[i][1]));
    }

    Vector2i P;
    ColourRGBA colour;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vector3 bc_screen  = barycentric(ToVec2(pts2[0]), ToVec2(pts2[1]), ToVec2(pts2[2]), P);
            Vector3 bc_clip    = Vector3(bc_screen.x/pts[0][3], bc_screen.y/pts[1][3], bc_screen.z/pts[2][3]);
            Vector3 barycentricCoord = bc_clip / (bc_clip.x + bc_clip.y + bc_clip.z);

            if ((bc_screen.x >= 0 && bc_screen.y >= 0 && bc_screen.z >= 0))
            {
                // Equivalent code
                float z = Vector3::Dot(ToVec3(clipc[2]), barycentricCoord); //float z = clipc[2] * barycentricCoord;

                if(framebuffer->DepthTest(P, z))
                {
                    bool discard = shader.fragment(eyeWorldPos, model, barycentricCoord, colour);
                    if (!discard)
                    {
                        framebuffer->SetPixel(P.x, height - P.y, colour);
                        framebuffer->SetDepth(P, z);
                    }
                }
            }
        }
    }
}

