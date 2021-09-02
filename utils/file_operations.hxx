#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "membuf.hxx"

namespace grottans {

/*! \brief open and load whole file
    \param[in] path - path to file
    \param[in] mode - mode of file opening 'rb', etc.
    \return grottans::membuf
*/
membuf open_load_file_to_membuf(std::string_view path, const std::string& mode);

/*! \brief filter parts of all lines after comment
    \param[in] data - grottans::membuf
    \param[in] comment - string which represent a comment
    \return std::stringstream
*/
std::stringstream filter_comments_in_membuf(membuf& data, const std::string& comment);

/*! \brief save file into filesystem
    \param[in] path - path to file
    \param[in] input_ss - stringstream with data
    \return EXIT_SUCCESS or runtime error
*/
int save_file_from_sstream(const std::string& path, std::stringstream input_ss);

} // end of the namespace