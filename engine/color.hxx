#pragma once

#include <cstdint>
#include <istream>

namespace grottans {

/*!
\file
\class 'color' class description

this class presents RGB color with Alfa-channel
with standart get/set methods and 
reloaded operators>>, operator<<
*/
class color {
public:
    /*! \brief default constructor*/
    color() = default;

    /*! \brief explicit constructor*/
    explicit color(const std::uint32_t& rgba_);

    /*! \brief RGBA constructor*/
    color(const float& r, const float& g, const float& b, const float& a);

    /*! \brief return current R value*/
    float get_r() const;

    /*! \brief return current G value*/
    float get_g() const;

    /*! \brief return current B value*/
    float get_b() const;

    /*! \brief return current A value*/
    float get_a() const;

    /*! \brief set new R value
        \param[in] r new value*/
    void set_r(const float& r);
    /*! \brief set new G value
        \param[in] g new value*/
    void set_g(const float& g);
    /*! \brief set new B value
        \param[in] b new value*/
    void set_b(const float& b);
    /*! \brief set new A value
        \param[in] a new value*/
    void set_a(const float& a);

private:
    std::uint32_t rgba = 0;

    friend std::istream& operator>>(std::istream& is, color&);
    friend std::ostream& operator<< (std::ostream& out, const color& p);
};

} // end of namespace
