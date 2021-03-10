#include "SystemTests.h"

// Math Library Tests.
void tests_dot()
{
    std:: cout << "Dot Product Tests" << std::endl;
    Vector4 a = Vector4(1,2,3,4);
    Vector4 b = Vector4(1,5,7,9);
    float c = Vector4::Dot(a,b);
    assert(c == 68);

    Vector3 d = Vector3(1,2,3);
    Vector3 e = Vector3(1,5,7);
    float f = Vector4::Dot(d,e);
    assert(f == 32);

    Vector2 g = Vector2(4,8);
    Vector2 h = Vector2(12,3);
    float i = Vector2::Dot(g,h);
    assert(i == 72);

    Vector2i j = Vector2i(7,9);
    Vector2i k = Vector2i(2,14);
    float l = Vector2i::Dot(j,k);
    assert(l == 140);
}

void tests_vecadd()
{
    std:: cout << "Vector Addition Tests" << std::endl;
    Vector4 a = Vector4(1,2,3,4);
    Vector4 b = Vector4(1,5,7,9);
    Vector4 c = Vector4::Add(a,b);
    assert(c.x == 2 && c.y == 7 && c.z == 10 && c.w == 13);

    Vector3 d = Vector3(1,2,3);
    Vector3 e = Vector3(1,5,7);
    Vector3 f = Vector3::Add(d,e);
    assert(f.x == 2 && f.y == 7 && f.z == 10);

    Vector2 g = Vector2(4,8);
    Vector2 h = Vector2(12,3);
    Vector2 i = Vector2::Add(g,h);
    assert(i.x == 16 && i.y == 11);

    Vector2i j = Vector2i(7,9);
    Vector2i k = Vector2i(2,14);
    Vector2i l = Vector2i::Add(j,k);
    assert(l.x == 9 && l.y == 23);

    Vector2i m = Vector2i(7,9);
    int n = 5;
    Vector2i o = Vector2i::Add(m,n);
    assert(o.x == 12 && o.y == 14);
}

void tests_vecsub()
{
    std:: cout << "Vector Subtraction Tests" << std::endl;
    Vector4 a = Vector4(1,2,3,4);
    Vector4 b = Vector4(1,5,7,9);
    Vector4 c = Vector4::Subtract(a,b);
    assert(c.x == 0 && c.y == -3 && c.z == -4 && c.w == -5);

    Vector3 d = Vector3(1,2,3);
    Vector3 e = Vector3(1,5,7);
    Vector3 f = Vector4::Subtract(d,e);
    assert(f.x == 0 && f.y == -3 && f.z == -4);

    Vector2 g = Vector2(4,8);
    Vector2 h = Vector2(12,3);
    Vector2 i = Vector2::Subtract(g,h);
    assert(i.x == -8 && i.y == 5);

    Vector2i j = Vector2i(7,9);
    Vector2i k = Vector2i(2,14);
    Vector2i l = Vector2i::Subtract(j,k);
    assert(l.x == 5 && l.y == -5);

    Vector2i m = Vector2i(7,9);
    int n = 5;
    Vector2i o = Vector2i::Subtract(m,n);
    assert(o.x == 2 && o.y == 4);
}

void tests_mat4()
{
    std:: cout << "Matrix4 Tests" << std::endl;
    Matrix4 m = Matrix4::GetIdentity();

    m.Translate(Vector3(2,3,4));
    Vector4 Row3 = m.GetRow3();
    assert(Row3.x == 2 && Row3.y == 3 && Row3.z == 4);
}

void system_assertions()
{
    tests_dot(); // Dot Product Tests
    tests_vecadd(); // Vector Addition Tests
    tests_vecsub(); // Vector Subtraction Tests
    tests_mat4(); // Matrix4 Tests
    std:: cout << "Passed All Tests." << std::endl;
}
