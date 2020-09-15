// abstraction

#include "./file_handler.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

file_handler::file_handler(file_handler_impl* p)
    : pimpl(p)
{
}

file_handler::~file_handler()
{
    delete pimpl;
}

SDL_file_handler::SDL_file_handler(const std::string& file_name)
    : file_handler(
#ifdef MT
          new MT_file_handler_impl()
#else
          new ST_file_handler_impl()
#endif
    )
    , file(file_name)
{
}

void SDL_file_handler::open(const std::string& file)
{
    pimpl->file_open(file);
}

} // end of namespace grottans
