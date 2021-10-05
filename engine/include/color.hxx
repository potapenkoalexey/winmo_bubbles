#pragma once

#include <cstdint>
#include <istream>

namespace grottans {

/*!
\file
\class 'color' class description

this class presents RGB color with Alfa-channel
with standart get/set methods and 
reloaded operator>>, operator<<
*/
class color {
public:
    /*! \brief default constructor*/
    color() = default;

    /*! \brief explicit constructor
        \param[in] rgba_ 32 bit variable each byte is represent one of the channels
    */
    explicit color(const std::uint32_t& rgba_);

    /*! \brief RGBA constructor with float parameters
        \param[in] r float value of R-part of the color
        \param[in] g float value of G-part of the color
        \param[in] b float value of B-part of the color
        \param[in] a float value of A-channel  
    */
    color(const float& r, const float& g, const float& b, const float& a);

    /*! \brief get value of the R-part of the color
        \return float
    */
    float get_r() const;
    
    /*! \brief get value of the G-part of the color
        \return float
    */
    float get_g() const;
    
    /*! \brief get value of the B-part of the color
        \return float
    */
    float get_b() const;
    
    /*! \brief get value of the A-part of the color
        \return float
    */
    float get_a() const;

    /*! \brief set value of the R-part of the color
        \param[in] R float value of R-part
    */
    void set_r(const float& r);

    /*! \brief set value of the G-part of the color
        \param[in] G float value of R-part
    */
    void set_g(const float& g);
    
    /*! \brief set value of the B-part of the color
        \param[in] B float value of R-part
    */
    void set_b(const float& b);
    
    /*! \brief set value of the A-part of the color
        \param[in] A float value of Alpha channel
    */
    void set_a(const float& a);

private:
    std::uint32_t rgba = 0; /*!< 32bit uint value of the RGBA-color */

    friend std::istream& operator>>(std::istream& is, color&);
    friend std::ostream& operator<< (std::ostream& out, const color& p);
};

} // end of namespace
