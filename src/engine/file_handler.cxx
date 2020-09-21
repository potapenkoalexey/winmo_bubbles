// abstraction

#include "./file_handler.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

file_handler::file_handler()
    : pimpl{ nullptr }
{
}

file_handler::~file_handler()
{
    if (pimpl)
        delete pimpl;
}


determine_file_handler_type::determine_file_handler_type(const file_type& type)
    : file_handler(
    )
{
    switch (type) {
    case grottans::file_type::txt: {
        pimpl = new text_file_handler_impl();
        break;
    }
    case grottans::file_type::dat: {
        pimpl = new text_file_handler_impl();
        break;
    }
    case grottans::file_type::ini: {
        pimpl = new ini_file_handler_impl();
        break;
    }
    }
}

void determine_file_handler_type::open(const std::string& file_name)
{
    pimpl->open_and_read_all_file_to_strstream(file_name);
}

void determine_file_handler_type::filter_comments()
{
    pimpl->filter_comments();
}

} // end of namespace grottans
