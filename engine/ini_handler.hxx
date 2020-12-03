#pragma once

#include <fstream>
#include <set>
#include <string>
#include <map>

namespace grottans {

/*!
\file
\class 'ini_handler' class description
*/
class ini_handler {
public:
    ini_handler(const std::string& filename);
    ~ini_handler();

    ini_handler(const ini_handler&) = delete;
    const ini_handler& operator=(const ini_handler&) = delete;
    ini_handler(ini_handler&&) = delete;
    ini_handler& operator=(ini_handler&&) = delete;

    // will return true if an error occurred
    bool error_check();

    /*! \brief sets the value into to key in section, will create/modify a section/key/value
        \param[in] section - section in the ini file
        \param[in] key - name of the key
        \param[in] value - value
    */
    void set(const std::string& section, const std::string& key, const std::string& value);
    void set_int(const std::string& section, const std::string& key, const unsigned long long& value);
    void set_real(const std::string& section, const std::string& key, const float& value);
    void set_boolean(const std::string& section, const std::string& key, const bool& value);

    /*! \brief gets the value from key in section
        \param[in] section - section in the ini file
        \param[in] key - name of the key
        \return[out] return a empty string, if there isn't any section or key
    */
    std::string get(const std::string& section, const std::string& key);

    /*! \brief gets the value (int, float, bool from key in section
        \param[in] section - section in the ini file
        \param[in] key - name of the key
        \return[out] return 0, if there isn't any section or key
    */
    unsigned long long get_int(const std::string& section, const std::string& key);
    float get_real(const std::string& section, const std::string& key);
    bool get_boolean(const std::string& section, const std::string& key);

    // section func
    // this function will return a set of unique strings
    std::set<std::string> get_section() const;

    // this function will return a set of unique strings
    std::set<std::string> get_fields(const std::string& section);

    // delete/close function
    // will return true if is successful
    bool del_key(const std::string& section, const std::string& key);

    bool del_section(const std::string& section);

    // write new values
    bool update();

    void save_settings_to_file();

    bool load_saved_settings();
    bool load_original_settings();

private:
    std::string filename;
    std::fstream file;
    std::map<std::string, std::map<std::string, std::string> > config;
    bool error;

    // function to get the current section;
    std::string s_get_section(const std::string& line);

    // function to get the current key and its value;
    bool get_line(const std::string& line, std::string& key, std::string& value);
};

} // end of namespace
