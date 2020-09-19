#pragma once

#include <sstream>
#include <string>

namespace grottans {

class file_handler_impl {
public:
    virtual void open_and_read_all_file_to_strstream(const std::string& file) = 0;
    virtual ~file_handler_impl() = default;
};

// inherited classes
class ST_file_handler_impl : public file_handler_impl {
public:
    void open_and_read_all_file_to_strstream(const std::string& file) override;

private:
    std::stringstream file_stream;
};

class MT_file_handler_impl : public file_handler_impl {
public:
    void open_and_read_all_file_to_strstream(const std::string& file) override;
};

} // end of namespace grottans
