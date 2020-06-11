#pragma once

#include <fstream>
#include <set>
#include <string>
#include <unordered_map>

namespace grottans {

class ini_handler {
public:
    ini_handler(const std::string& filename);
    ~ini_handler()
    {
        //save_settings_to_file();
    }

    ini_handler(const ini_handler&) = delete;
    const ini_handler& operator=(const ini_handler&) = delete;
    ini_handler(ini_handler&&) = delete;
    ini_handler& operator=(ini_handler&&) = delete;

    // will return true if an error occurred
    bool error_check();

    // set functions
    // set will create/modify a section/key/value, same for the similar functions
    void set(const std::string& section, const std::string& key, const std::string& value);
    void set_int(const std::string& section, const std::string& key, const unsigned long long& value);
    void set_real(const std::string& section, const std::string& key, const float& value);
    void set_boolean(const std::string& section, const std::string& key, const bool& value);

    bool update();

    // read functions
    // will return a empty string, if there isn't any section or key
    std::string get(const std::string& section, const std::string& key);

    // will return 0, if there isn't any section or key
    unsigned long long get_int(const std::string& section, const std::string& key);

    // will return 0, if there isn't any section or key
    float get_real(const std::string& section, const std::string& key);

    // will return 0, if there isn't any section or key
    bool get_boolean(const std::string& section, const std::string& key);

    // section func
    // this function will return a set of unique strings
    std::set<std::string> get_section() const;

    // this function will return a set of unique strings
    std::set<std::string> get_fields(const std::string& section);

    // delete/close function
    // will return true if is sucessful
    bool del_key(const std::string& section, const std::string& key);

    // return true if is sucessful
    bool del_section(const std::string& section);

    // Will add the new content to the file
    void save_settings_to_file();

private:
    std::string filename;
    std::fstream file;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string> > config;
    bool error;

    // function to get the current section;
    std::string s_get_section(const std::string& line);

    // function to get the current key and its value;
    bool get_line(const std::string& line, std::string& key, std::string& value);
};

} // end of namespace
