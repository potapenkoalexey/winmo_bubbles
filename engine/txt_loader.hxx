#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "./engine.hxx"

std::stringstream filter_comments(std::string_view file, const std::string& comment)
{
    std::stringstream out;
    std::string line;
    //std::ifstream     in(file.data(), std::ios_base::binary);
    grottans::membuf buf = grottans::load_file(file);
    std::istream in(&buf);

    if (!in) {
        throw std::runtime_error(std::string("can't open file: ") + file.data());
    }

    while (std::getline(in, line)) {
        size_t comment_pos = line.find(comment);
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        if (!line.empty()) {
            out << line << '\n';
        }
    }

    return out;
}
