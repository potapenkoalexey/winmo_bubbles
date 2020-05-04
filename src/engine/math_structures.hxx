#pragma once

#include "./color.hxx"

namespace grottans {

struct vec2 {
    vec2();
    vec2(const float& x, const float& y);
    float x = 0;
    float y = 0;
};

vec2 operator+(const vec2& l, const vec2& r);

struct mat2 {
    mat2();
    static mat2 identiry();
    static mat2 scale(const float& scale);
    static mat2 rotation(const float& thetha);
    vec2 col0;
    vec2 col1;
};

struct mat2x3 {
    mat2x3();
    static mat2x3 identiry();
    static mat2x3 scale(const float& scale);
    static mat2x3 scale(const float& sx, const float& sy);
    static mat2x3 rotation(const float& thetha);
    static mat2x3 move(const vec2& delta);
    vec2 col0;
    vec2 col1;
    vec2 delta;
};

vec2 operator*(const vec2& v, const mat2& m);
mat2 operator*(const mat2& m1, const mat2& m2);
vec2 operator*(const vec2& v, const mat2x3& m);
mat2x3 operator*(const mat2x3& m1, const mat2x3& m2);

/// vertex with position only
struct v0 {
    vec2 pos;
};

/// vertex with position and texture coordinate
struct v1 {
    vec2 pos;
    color c;
};
/// vertex position + color + texture coordinate
struct v2 {
    vec2 pos;
    vec2 uv;
    color c;
};

/// triangle with positions only
struct tri0 {
    tri0();
    v0 v[3];
};

/// triangle with positions and color
struct tri1 {
    tri1();
    v1 v[3];
};

/// triangle with positions color and texture coordinate
struct tri2 {
    tri2();
    v2 v[3];
};

v2 operator+(const v2& l, const v2& r);
tri2 operator+(const tri2& l, const tri2& r);

std::istream& operator>>(std::istream& is, v0&);
std::istream& operator>>(std::istream& is, v1&);
std::istream& operator>>(std::istream& is, v2&);
std::istream& operator>>(std::istream& is, tri0&);
std::istream& operator>>(std::istream& is, tri1&);
std::istream& operator>>(std::istream& is, tri2&);

} // end of namespace
