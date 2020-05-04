#pragma once

#include "math_structures.hxx"

namespace grottans {

class vertex_buffer {
public:
    virtual const v2* data() const = 0;
    virtual size_t size() const = 0;

    vertex_buffer() = default;
    virtual ~vertex_buffer();
    vertex_buffer(vertex_buffer const&) = delete;
    vertex_buffer& operator=(vertex_buffer const&) = delete;
};

} //end of namespace
