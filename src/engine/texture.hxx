#pragma once

#include <iostream>

namespace grottans {

class texture {
public:
    virtual std::uint32_t get_width() const = 0;
    virtual std::uint32_t get_height() const = 0;

    texture() = default;
    virtual ~texture();
    texture(texture const&) = delete;
    texture& operator=(texture const&) = delete;
};

} //end of namespace
