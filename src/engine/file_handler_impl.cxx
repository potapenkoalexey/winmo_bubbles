#include <iostream>

#include "./file_handler_impl.hxx"

namespace grottans {

void ST_file_handler_impl::file_open(const std::string& file)
{
    std::cout << "Single-threaded file logger" << file << std::endl;
}

void MT_file_handler_impl::file_open(const std::string& file)
{
    std::cout << "Multi-threaded file logger" << file << std::endl;
}

} // end of namespace grottans
