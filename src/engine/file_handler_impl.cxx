#include <iostream>

#include "./engine.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

// maybe file handlers for different OS make a sense?
// POSIX_f_h_i
// Win_f_h_i
// Android_f_h_i
// or by file types ??? INI_f_h_i OR JSON_f_h_i AND etc

void ST_file_handler_impl::open_and_read_all_file_to_strstream(const std::string& file)
{
    // change it to call of on of the logger type (console)
    std::cout << "-- Single-threaded file loader" << file << std::endl;

    std::stringstream out;
    std::string line;
    membuf buf = load_file(file);
    std::istream in(&buf);

    while (std::getline(in, line)) {
        if (!line.empty()) {
            out << line << '\n';
        }
    }
}

void MT_file_handler_impl::open_and_read_all_file_to_strstream(const std::string& file)
{
    // change it to call of on of the logger type (console)
    std::cout << "-- Multi-threaded file loader" << file << std::endl;
}

} // end of namespace grottans
