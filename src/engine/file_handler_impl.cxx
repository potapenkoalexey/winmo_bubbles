#include <iostream>

#include "./engine.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

// maybe file handlers for different OS make a sense?
// POSIX_f_h_i
// Win_f_h_i
// Android_f_h_i
// or by file types ??? INI_f_h_i OR JSON_f_h_i AND etc

void text_file_handler_impl::open_and_read_all_file_to_strstream(const std::string& file)
{
    // change it to call of on of the logger type (console)
    std::cout << "-- Single-threaded TEXT file loader open and read file: " << file << std::endl;

    std::string line;
    membuf buf = load_file(file);
    std::istream in(&buf);

    while (std::getline(in, line)) {
        if (!line.empty()) {
            file_stream << line << '\n';
        }
    }
}

void text_file_handler_impl::filter_comments()
{
    std::stringstream out;
    std::string line;

    if (!file_stream) {
        throw std::runtime_error(std::string("-- Internal buffer is clear\n"));
    }

    while (std::getline(file_stream, line)) {
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (!line.empty()) {
            out << line << '\n';
        }
    }

    file_stream.swap(out);
}

void ini_file_handler_impl::open_and_read_all_file_to_strstream(const std::string& file)
{
    // change it to call of on of the logger type (console)
    std::cout << "-- Single-threaded INI file loader open and read file: " << file << std::endl;
}

void ini_file_handler_impl::filter_comments()
{
    std::stringstream out;
    std::string line;

    if (!file_stream) {
        throw std::runtime_error(std::string("-- Internal buffer is clear\n"));
    }

    while (std::getline(file_stream, line)) {
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (!line.empty()) {
            out << line << '\n';
        }
    }

    file_stream.swap(out);
}

} // end of namespace grottans
