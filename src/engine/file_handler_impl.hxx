#pragma once

#include <string>

namespace grottans {

class file_handler_impl {
public:
    virtual void file_open(const std::string& file) = 0;
    virtual ~file_handler_impl();
};

class ST_file_handler_impl : public file_handler_impl {
public:
    void file_open(const std::string& file) override;
};

class MT_file_handler_impl : public file_handler_impl {
public:
    void file_open(const std::string& file) override;
};

} // end of namespace grottans
