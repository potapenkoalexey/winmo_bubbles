#pragma once

#include <string>

#include "./engine.hxx"
#include "./file_handler_impl.hxx"

namespace grottans {

// leading declaration

class file_handler {
public:
    file_handler(file_handler_impl* p);
    virtual ~file_handler();
    virtual void open() = 0;
    //virtual void read(const std::string& str) = 0;
    //virtual void write(const std::string& str) = 0;
    //virtual void close() = 0;

protected:
    file_handler_impl* pimpl;
};

///////////////////////////////////////////////////////////////////////////////
class SDL_file_handler : public file_handler {
public:
    SDL_file_handler(const std::string& file_name);
    void open() override;
    //void read(const std::string& str);
    //void write(const std::string& str);
    //void close();

private:
    std::string file;
};

} // end of namespace grottans
