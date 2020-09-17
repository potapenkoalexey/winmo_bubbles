#pragma once

#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "./engine.hxx"

std::stringstream filter_comments(std::string_view file)
{
    std::stringstream out;
    std::string line;
    grottans::membuf buf = grottans::load_file(file);
    std::istream in(&buf);

    if (!in) {
        throw std::runtime_error(std::string("can't open file: ") + file.data());
    }

    while (std::getline(in, line)) {
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (!line.empty()) {
            out << line << '\n';
        }
    }

    return out;
}
