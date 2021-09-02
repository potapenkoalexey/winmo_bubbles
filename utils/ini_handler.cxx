#include <iostream>
#include <sstream>
#include <string>

#include "ini_handler.hxx"
#include "SDL.h"

#include "file_operations.hxx"
#include "membuf.hxx"
#include "../game/global_variables.hxx"

namespace grottans {

ini_handler::ini_handler(const std::string& filename_)
{
    // We initialize our variable.
    filename = filename_;
    error = false;
    
    membuf input_mbuf = open_load_file_to_membuf(filename_, "rb");
    std::stringstream file_ss = filter_comments_in_membuf(input_mbuf, ";");

    std::string actual_line = "";
    std::string actual_section = "";
    std::string key = "";
    std::string value = "";

    bool in_section = false;

    // To save our current section
    std::map<std::string, std::string> section;

    if (file_ss) {
        while (std::getline(file_ss, actual_line)) {

            if (!actual_line.size())
                continue;

            // if section
            if (actual_line[0] == '[') {
                if (actual_section != s_get_section(actual_line)) {
                    // Not empty section or first section/line
                    if (!section.empty() && !actual_section.empty()) {
                        // We add to our file the finished section
                        config[actual_section] = section;

                        // Clear the current section
                        section.clear();
                    }
                    // Set new section
                    actual_section = s_get_section(actual_line);
                }
                in_section = true;
            } else {
                // in_section
                if (in_section) {
                    // get line
                    if (parse_line(actual_line, key, value)) {
                        std::cout << "-- INI: " << actual_section << ": " << key << " = " << value << std::endl;
                        section[key] = value;
                    } else {
                        // Is this a commentary? do nothing
                    }
                }
            }
        }
        // Add last section
        if (!section.empty()) {
            config[actual_section] = section;
            section.clear();
        }
    } else {
        throw std::runtime_error("Can't open settings.ini");
    }
}

ini_handler::~ini_handler()
{
    // these operations runs throught the engine 
    // update();
    // save_settings_to_file();
    
    // clear the old content
    config.clear();
    filename.clear();
    if (file.is_open())
        file.close();
}

void ini_handler::save_settings_to_file()
{
    if (filename.size() == 0)
    {
        throw std::runtime_error("ini parser: setting do not exist in memory!\n");
    }

    std::stringstream ss;

    size_t count{ 0 };
    size_t total_section{ config.size() };
    size_t total_key{ 0 };
    size_t key_count{ 0 };

    // start to go through every section
    for (auto section : config) {
        ++count;
        key_count = 0;

        ss << "[" << section.first << "]";
#ifdef __unix__
        ss << std::endl;
#endif
#ifdef _WIN32
        ss << '\r' << std::endl;
#endif
         
        total_key = section.second.size();

        // start to go through every key
        for (auto key : section.second) {
            ss << key.first << "=" << key.second << std::endl;
        }
    }

    // need to work on Android
    grottans::save_file_from_sstream(filename.c_str(), std::move(ss));
}

std::string end_of_the_line_crossplatform(const std::string& input){
#ifdef __unix__
    return input;
#endif
#ifdef _WIN32
    return input + "\r";
#endif
}

bool ini_handler::update()
{
    if (g_MODE == MODE::classic)
        set("Saved", "g_MODE", end_of_the_line_crossplatform("1"));
    else if (g_MODE == MODE::extreme)
        set("Saved", "g_MODE", end_of_the_line_crossplatform("2"));
    else
        set("Saved", "g_MODE", end_of_the_line_crossplatform("0"));

    set("Saved", "g_LEVEL", end_of_the_line_crossplatform(std::to_string(g_LEVEL)));
    set("Saved", "g_SCORE", end_of_the_line_crossplatform(std::to_string(g_SCORE)));

    set("Saved", "g_SCORE_MAX_CLASSIC", end_of_the_line_crossplatform(std::to_string(g_SCORE_MAX_CLASSIC)));
    set("Saved", "g_SCORE_MAX_EXTREME", end_of_the_line_crossplatform(std::to_string(g_SCORE_MAX_EXTREME)));

    set("Saved", "g_score_in_the_end_of_level", end_of_the_line_crossplatform(std::to_string(g_score_in_the_end_of_level)));
    set_boolean("Saved", "g_SOUND", g_SOUND);

    return true;
}

bool ini_handler::load_settings(const std::string& section)
{
    unsigned long long temp = get_int(section, "g_MODE");
    if (temp == 0)
        g_MODE = MODE::non;
    else if (temp == 1)
        g_MODE = MODE::classic;
    else if (temp == 2)
        g_MODE = MODE::extreme;

    g_LEVEL = static_cast<size_t>(get_int(section, "g_LEVEL"));
    g_SCORE = static_cast<size_t>(get_int(section, "g_SCORE"));
    g_SCORE_MAX_CLASSIC = static_cast<size_t>(get_int(section, "g_SCORE_MAX_CLASSIC"));
    g_SCORE_MAX_EXTREME = static_cast<size_t>(get_int(section, "g_SCORE_MAX_EXTREME"));
    g_score_in_the_end_of_level = static_cast<size_t>(get_int(section, "g_score_in_the_end_of_level"));
    g_SOUND = get_boolean(section, "g_SOUND");

    return true;
}

bool ini_handler::error_check()
{
    ///
    ///
    return error;
}

void ini_handler::set(const std::string& section, const std::string& key, const std::string& value)
{
    config[section][key] = value;
}

void ini_handler::set_int(const std::string& section, const std::string& key, const unsigned long long& value)
{
    set(section, key, std::to_string(value));
}

void ini_handler::set_real(const std::string& section, const std::string& key, const float& value)
{
    set(section, key, std::to_string(value));
}

void ini_handler::set_boolean(const std::string& section, const std::string& key, const bool& value)
{
    if (value)
        set(section, key, end_of_the_line_crossplatform("true"));
    else
        set(section, key, end_of_the_line_crossplatform("false"));
}

std::string ini_handler::get(const std::string& section, const std::string& key)
{
    if (!config.count(section) || !config[section].count(key)) {
        return "";
    }
    return config[section][key];
}

unsigned long long ini_handler::get_int(const std::string& section, const std::string& key)
{
    std::string::size_type sz{ 0 };
    const std::string str = get(section, key);

    return str.size() ? std::stoull(str, &sz, 0) : 0;
}

float ini_handler::get_real(const std::string& section, const std::string& key)
{
    std::string::size_type sz{ 0 };
    const std::string str = get(section, key);

    return str.size() ? static_cast<float>(std::stod(str, &sz)) : 0;
}

bool ini_handler::get_boolean(const std::string& section, const std::string& key)
{
    const std::string str = get(section, key);
    return str == "true" ? true : false;
}

std::set<std::string> ini_handler::get_section() const
{
    std::set<std::string> buffer;
    for (auto section : config) {
        buffer.insert(section.first);
    }
    return buffer;
}

std::set<std::string> ini_handler::get_fields(const std::string& section)
{
    std::set<std::string> buffer;

    if (!config.count(section))
        return buffer;

    for (auto key : config[section]) {
        buffer.insert(key.first);
    }
    return buffer;
}

bool ini_handler::del_key(const std::string& section, const std::string& key)
{
    return config[section].erase(key);
}

bool ini_handler::del_section(const std::string& section)
{
    return config.erase(section);
}

std::string ini_handler::s_get_section(const std::string& line)
{
#ifdef __unix__
    std::string temp { line.begin() + 1, line.end() - 1 };
#endif
#ifdef _WIN32
    std::string temp { line.begin() + 1, line.end() - 2 };
#endif
    
    return temp;
}

bool ini_handler::parse_line(const std::string& line, std::string& key, std::string& value)
{
    auto found = line.find('=');
    if (found) {
        //this is valid.
        key = line.substr(0, found);
        value = line.substr(found + 1, line.size() - 1);

        return true;
    }

    return false;
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

} // end of namespace
