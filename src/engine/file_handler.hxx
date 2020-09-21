#pragma once

#include <string>

#include "./engine.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

// leading declaration

enum file_type{
    txt,
    dat,
    ini
};

class file_handler {
public:
    file_handler();
    virtual ~file_handler();
    virtual void open(const std::string& file_name) = 0;
    virtual void filter_comments() = 0;
    //virtual void read(const std::string& str) = 0;
    //virtual void write(const std::string& str) = 0;
    //virtual void close() = 0;

protected:
    file_handler_impl* pimpl;
};

///////////////////////////////////////////////////////////////////////////////
class determine_file_handler_type : public file_handler {
public:
    determine_file_handler_type(const file_type& type);
    void open(const std::string& file_name) override;
    void filter_comments() override;
    //void read(const std::string& str);
    //void write(const std::string& str);
    //void close();
};

} // end of namespace grottans
