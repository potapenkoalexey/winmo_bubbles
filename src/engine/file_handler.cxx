// abstraction

#include "./file_handler.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

file_handler::file_handler(/*file_handler_impl* p*/)
{
//    pimpl = p;
}

file_handler::~file_handler()
{
    delete pimpl;
}


common_file_handler::common_file_handler(const file_type& type)
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

void common_file_handler::open(const std::string& file_name)
{
    pimpl->open_and_read_all_file_to_strstream(file_name);
}

} // end of namespace grottans
