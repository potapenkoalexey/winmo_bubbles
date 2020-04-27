#include "./math_structures.hxx"

#include <cmath>

namespace grottans {

vec2::vec2()
    : x(0.f)
    , y(1.f)
{
}

vec2::vec2(float x_, float y_)
    : x(x_)
    , y(y_)
{
}

vec2 operator+(const vec2& l, const vec2& r)
{
    vec2 result;
    result.x = l.x + r.x;
    result.y = l.y + r.y;
    return result;
}

mat2::mat2()
    : col0(1.0f, 0.f)
    , col1(0.f, 1.f)
{
}

mat2x3::mat2x3()
    : col0(1.0f, 0.f)
    , col1(0.f, 1.f)
    , delta(0.f, 0.f)
{
}

mat2 mat2::identiry()
{
    return mat2::scale(1.f);
}

mat2x3 mat2x3::identiry()
{
    return mat2x3::scale(1.f);
}

mat2 mat2::scale(float scale)
{
    mat2 result;
    result.col0.x = scale;
    result.col1.y = scale;
    return result;
}

mat2x3 mat2x3::scale(float scale)
{
    mat2x3 result;
    result.col0.x = scale;
    result.col1.y = scale;
    return result;
}

mat2x3 mat2x3::scale(float sx, float sy)
{
    mat2x3 r;
    r.col0.x = sx;
    r.col1.y = sy;
    return r;
}

mat2 mat2::rotation(float thetha)
{
    mat2 result;

    result.col0.x = std::cos(thetha);
    result.col0.y = std::sin(thetha);

    result.col1.x = -std::sin(thetha);
    result.col1.y = std::cos(thetha);

    return result;
}

mat2x3 mat2x3::rotation(float thetha)
{
    mat2x3 result;

    result.col0.x = std::cos(thetha);
    result.col0.y = std::sin(thetha);

    result.col1.x = -std::sin(thetha);
    result.col1.y = std::cos(thetha);

    return result;
}

mat2x3 mat2x3::move(const vec2& delta)
{
    mat2x3 r = mat2x3::identiry();
    r.delta = delta;
    return r;
}

vec2 operator*(const vec2& v, const mat2& m)
{
    vec2 result;
    result.x = v.x * m.col0.x + v.y * m.col0.y;
    result.y = v.x * m.col1.x + v.y * m.col1.y;
    return result;
}

vec2 operator*(const vec2& v, const mat2x3& m)
{
    vec2 result;
    result.x = v.x * m.col0.x + v.y * m.col0.y + m.delta.x;
    result.y = v.x * m.col1.x + v.y * m.col1.y + m.delta.y;
    return result;
}

mat2x3 operator*(const mat2x3& m1, const mat2x3& m2)
{
    mat2x3 r;

    r.col0.x = m1.col0.x * m2.col0.x + m1.col1.x * m2.col0.y;
    r.col1.x = m1.col0.x * m2.col1.x + m1.col1.x * m2.col1.y;
    r.col0.y = m1.col0.y * m2.col0.x + m1.col1.y * m2.col0.y;
    r.col1.y = m1.col0.y * m2.col1.x + m1.col1.y * m2.col1.y;
    //dreco-engine
    r.delta.x = m1.col0.x * m2.delta.x + m1.col0.y * m2.delta.y + m1.delta.x;
    r.delta.y = m1.col1.x * m2.delta.x + m1.col1.y * m2.delta.y + m1.delta.y;
    //old
    // r.delta.x = m1.delta.x * m2.col0.x + m1.delta.y * m2.col0.y + m2.delta.x;
    // r.delta.y = m1.delta.x * m2.col1.x + m1.delta.y * m2.col1.y + m2.delta.y;

    return r;
}

mat2 operator*(const mat2& m1, const mat2& m2)
{
    mat2 r;

    r.col0.x = m1.col0.x * m2.col0.x + m1.col1.x * m2.col0.y;
    r.col1.x = m1.col0.x * m2.col1.x + m1.col1.x * m2.col1.y;
    r.col0.y = m1.col0.y * m2.col0.x + m1.col1.y * m2.col0.y;
    r.col1.y = m1.col0.y * m2.col1.x + m1.col1.y * m2.col1.y;

    return r;
}

std::istream& operator>>(std::istream& is, v0& v)
{
    is >> v.pos.x;
    is >> v.pos.y;

    return is;
}

std::istream& operator>>(std::istream& is, v1& v)
{
    is >> v.pos.x;
    is >> v.pos.y;
    is >> v.c;
    return is;
}

std::istream& operator>>(std::istream& is, v2& v)
{
    is >> v.pos.x;
    is >> v.pos.y;
    is >> v.uv.x;
    is >> v.uv.y;
    is >> v.c;
    return is;
}

std::istream& operator>>(std::istream& is, tri0& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

std::istream& operator>>(std::istream& is, tri1& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

std::istream& operator>>(std::istream& is, tri2& t)
{
    is >> t.v[0];
    is >> t.v[1];
    is >> t.v[2];
    return is;
}

tri0::tri0()
    : v{ v0(), v0(), v0() }
{
}

tri1::tri1()
    : v{ v1(), v1(), v1() }
{
}

tri2::tri2()
    : v{ v2(), v2(), v2() }
{
}

v2 operator+(const v2& l, const v2& r)
{
    v2 res;
    res.pos.x = l.pos.x + r.pos.x;
    res.pos.y = l.pos.y + r.pos.y;
    res.uv.x = l.uv.x + r.uv.x;
    res.uv.y = l.uv.y + r.uv.y;
    res.c.set_a(l.c.get_a() + r.c.get_a());
    res.c.set_r(l.c.get_r() + r.c.get_r());
    res.c.set_b(l.c.get_b() + r.c.get_b());
    res.c.set_g(l.c.get_g() + r.c.get_g());
    return res;
}

tri2 operator+(const tri2& l, const tri2& r)
{
    tri2 res;
    res.v[0] = l.v[0] + r.v[0];
    res.v[0] = l.v[0] + r.v[0];
    res.v[1] = l.v[1] + r.v[1];
    res.v[1] = l.v[1] + r.v[1];
    res.v[2] = l.v[2] + r.v[2];
    res.v[2] = l.v[2] + r.v[2];

    return res;
}
}
