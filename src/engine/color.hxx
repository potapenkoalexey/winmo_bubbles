#pragma once

#include <cstdint>
#include <istream>

namespace grottans {

class color {
public:
    color() = default;
    explicit color(const std::uint32_t& rgba_);
    color(const float& r, const float& g, const float& b, const float& a);

    float get_r() const;
    float get_g() const;
    float get_b() const;
    float get_a() const;

    void set_r(const float& r);
    void set_g(const float& g);
    void set_b(const float& b);
    void set_a(const float& a);

private:
    std::uint32_t rgba = 0;
};

std::istream& operator>>(std::istream& is, color&);

} // end of namespace
