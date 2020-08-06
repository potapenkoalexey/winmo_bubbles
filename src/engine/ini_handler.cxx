#include <iostream>

#include "ini_handler.hxx"
#include "../game/global_variables.hxx"

namespace grottans {

ini_handler::ini_handler(const std::string& filename_)
{
    // We initialize our variable.
    filename = filename_;
    error = false;

    file.open(filename, std::ios::in | std::ios::binary);

    std::string actual_line = "";
    std::string actual_section = "";
    std::string key = "";
    std::string value = "";

    bool in_section = false;

    // To save our current section
    std::map<std::string, std::string> section;

    if (this->file.is_open()) {
        while (std::getline(file, actual_line)) {

            if (!actual_line.size())
                continue;

            // if section
            if (actual_line[0] == '[' && actual_line[actual_line.size() - 1] == ']') {
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
                    if (get_line(actual_line, key, value)) {
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

        file.close();
    } else {
        error = true;
    }
}

ini_handler::~ini_handler()
{
    // clear the old content
    config.clear();
    filename.clear();
    file.close();
    update();
    save_settings_to_file();
}

void ini_handler::save_settings_to_file()
{
    if (filename.size() == 0)
        return;

    // already have our file in the buffer, so delete the content of file
    file.open(filename, std::ios::out | std::ios::binary);// std::ofstream::out | std::ofstream::trunc);

    // set up the content for the ending of file. Plus, handling the last empty line in the file
    size_t count{ 0 };
    size_t total_section{ config.size() };
    size_t total_key{ 0 };
    size_t key_count{ 0 };

    // start to go through every section
    for (auto section : config) {
        ++count;
        key_count = 0;

        file << '[' + section.first + ']' << std::endl;
        total_key = section.second.size();

        // start to go through every key
        for (auto key : section.second) {
            ++key_count;
            if (key_count != total_key) {
                file << key.first + "=" + key.second << std::endl;
            } else {
                file << key.first + "=" + key.second;
            }
        }
        if (count != total_section) {
            file << std::endl
                 << std::endl;
        }
    }

    error = false;
}

bool ini_handler::update()
{
    if (g_MODE == MODE::classic)
        set("Saved", "g_MODE", "1");
    else if (g_MODE == MODE::extreme)
        set("Saved", "g_MODE", "2");
    else
        set("Saved", "g_MODE", "0");

    set("Saved", "g_LEVEL", std::to_string(g_LEVEL));
    set("Saved", "g_SCORE", std::to_string(g_SCORE));
    set_boolean("Saved", "g_SOUND", g_SOUND);

    return true;
}

bool ini_handler::load_saved_settings()
{
    unsigned long long temp = get_int("Saved", "g_MODE");
    if (temp == 0)
        g_MODE = MODE::non;
    else if (temp == 1)
        g_MODE = MODE::classic;
    else if (temp == 2)
        g_MODE = MODE::extreme;

    g_LEVEL = static_cast<size_t>(get_int("Saved", "g_LEVEL"));
    g_SCORE = static_cast<size_t>(get_int("Saved", "g_SCORE"));
    g_SOUND = get_boolean("Saved", "g_SOUND");

    return true;
}

bool ini_handler::load_original_settings()
{
    unsigned long long temp = get_int("Original", "g_MODE");
    if (temp == 0)
        g_MODE = MODE::non;
    else if (temp == 1)
        g_MODE = MODE::classic;
    else if (temp == 2)
        g_MODE = MODE::extreme;

    g_LEVEL = static_cast<size_t>(get_int("Original", "g_LEVEL"));
    g_SCORE = static_cast<size_t>(get_int("Original", "g_SCORE"));
    g_SOUND = get_boolean("Original", "g_SOUND");
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
        set(section, key, "true");
    else
        set(section, key, "false");
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
    std::string temp = line;
    temp.erase(temp.begin());
    temp.erase(temp.end() - 1);
    return temp;
}

bool ini_handler::get_line(const std::string& line, std::string& key, std::string& value)
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

} // end of namespace
