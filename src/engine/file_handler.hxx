#pragma once

#include <string>

#include "./engine.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

// leading declaration

enum file_type{
    txt,
    dat,
    ini,
};

class file_handler {
public:
    file_handler(/*file_handler_impl* p*/);
    virtual ~file_handler();
    virtual void open(const std::string& file_name) = 0;
    //virtual void read(const std::string& str) = 0;
    //virtual void write(const std::string& str) = 0;
    //virtual void close() = 0;

protected:
    file_handler_impl* pimpl;
};

///////////////////////////////////////////////////////////////////////////////
class common_file_handler : public file_handler {
public:
    common_file_handler(const file_type& type);
    void open(const std::string& file_name) override;
    //void read(const std::string& str);
    //void write(const std::string& str);
    //void close();
};

} // end of namespace grottans
