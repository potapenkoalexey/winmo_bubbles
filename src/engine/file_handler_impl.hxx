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
class text_file_handler_impl final : public file_handler_impl {
public:
    void open_and_read_all_file_to_strstream(const std::string& file) override;
private:
    std::string file_name;
    std::stringstream file_stream;
};

class ini_file_handler_impl final : public file_handler_impl {
public:
    void open_and_read_all_file_to_strstream(const std::string& file) override;
private:
    std::string file_name;
    std::stringstream file_stream;
};

} // end of namespace grottans
