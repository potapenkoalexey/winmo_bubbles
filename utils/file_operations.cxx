#include "include/file_operations.hxx"

#include "SDL.h"

namespace grottans {

/*! \brief open and load whole file
    \param[in] path - path to file
    \param[in] mode - mode of file opening 'rb', etc.
    \return grottans::membuf
*/
membuf open_load_file_to_membuf(std::string_view path, const std::string& mode) 
{
    SDL_RWops* io = SDL_RWFromFile(path.data(), mode.c_str()); // "rb"
    if (nullptr == io) {
        throw std::runtime_error("can't load file: " + std::string(path));
    }

    Sint64 file_size = io->size(io);
    if (-1 == file_size) {
        throw std::runtime_error("can't determine size of file: " + std::string(path));
    }
    size_t size = static_cast<size_t>(file_size);
    std::unique_ptr<char[]> mem = std::make_unique<char[]>(size);

    size_t num_readed_objects = io->read(io, mem.get(), size, 1);
    if (num_readed_objects != 1) {
        throw std::runtime_error("can't read all content from file: " + std::string(path));
    }

    if (0 != io->close(io)) {
        throw std::runtime_error("failed close file: " + std::string(path));
    }
    return membuf(std::move(mem), size);
}

/*! \brief filter parts of all lines after comment
    \param[in] data - grottans::membuf
    \param[in] comment - string which represent a comment
    \return std::stringstream
*/
std::stringstream filter_comments_in_membuf(membuf& data, const std::string& comment)
{
    std::stringstream out;
    std::string line;
    std::istream in(&data);

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

/*! \brief save file into filesystem
    \param[in] path - path to file
    \param[in] input_ss - stringstream with data
    \return EXIT_SUCCESS or runtime error
*/
int save_file_from_sstream(const std::string& path, std::stringstream input_ss)
{
    // https://wiki.libsdl.org/SDL_GetPrefPath
    // char* SDL_GetPrefPath(const char *org, const char *app);

    SDL_RWops* output = SDL_RWFromFile(path.c_str(), "w");
    if(!output){
        throw std::runtime_error("Cannot open: " + path);
    }
    std::string tmp = input_ss.rdbuf()->str();
    size_t size = tmp.size();
    const char* tmp_char = tmp.c_str();


    // on Android you need to create local file before write on if !!!
    // see app/jni/src/thirdparty/SDL2-2.0.16/src/core/android/SDL_android.c:1889


    if(SDL_RWwrite(output, (const void*)tmp_char, size, 1)){
        std::cout << "\n File saved!\n" << path << std::endl;
    } else {
        throw std::runtime_error("Cannot write into: " + path);
    }

    SDL_RWclose(output);

    return EXIT_SUCCESS;
}

} // end of the namespace