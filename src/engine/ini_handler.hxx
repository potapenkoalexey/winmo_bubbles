#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <set>

namespace grottans {

class ini_handler
{
public:
    ini_handler(const std::string& filename);
    ~ini_handler(){
        close();
    }

    ini_handler(const ini_handler&) = delete;
    const ini_handler& operator=(const ini_handler&) = delete;
    ini_handler(ini_handler&&) = delete;
    ini_handler& operator=(ini_handler&&) = delete;

    // Will return true if an error occurred.
    bool error_check();


    // Set functions

    // Set will create/modify a section/key/value, same for the similar functions.
    void set(const std::string& section,const std::string& key,const std::string& value);
    void set_int(const std::string& section,const std::string& key,const unsigned long long& value);
    void set_real(const std::string& section,const std::string& key,const double& value);
    void set_boolean(const std::string& section,const std::string& key,const bool& value);

    // Read functions.

    // This function will return a empty string, if there is no section or key.
    std::string get(const std::string& section, const std::string& key);

    // This will return 0, if there is no section or key.
    unsigned long long get_int(const std::string& section, const std::string& key);

    // This will return 0, if there is no section or key.
    double get_real(const std::string& section, const std::string& key);

    // This will return 0, if there is no section or key.
    bool get_boolean(const std::string& section, const std::string& key);

    // Section func

    // This function will return a set of unique strings.
    std::set<std::string> get_section() const;

    // This function will return a set of unique strings;
    std::set<std::string> get_fields(const std::string& section);

    // Delete/close function

    // This function will return true if is sucessful.
    bool del_key(const std::string& section, const std::string& key);

    // This function will return true if is sucessful.
    bool del_section(const std::string& section);

    // Will add the new content to the file.
    void close();

private:
    // Variables
    std::string filename;
    std::fstream file;
    std::unordered_map<std::string, std::unordered_map<std::string,std::string>> config;
    bool error;


    // snippets

    // function to get the current section;
    inline std::string s_get_section(std::string line) const;

    // function to get the current key and its value;
    bool get_line(const std::string& line, std::string& key, std::string& value);
};

} // end of namespace
