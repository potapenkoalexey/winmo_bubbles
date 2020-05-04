#pragma once

#include <memory>
#include <streambuf>

namespace grottans {

struct membuf : public std::streambuf {
    membuf()
        : std::streambuf()
        , buf()
        , buf_size{ 0 }
    {
    }

    membuf(std::unique_ptr<char[]> buffer, size_t size)
        : std::streambuf()
        , buf(std::move(buffer))
        , buf_size{ size }
    {
        char* beg_ptr = buf.get();
        char* end_ptr = beg_ptr + buf_size;
        setg(beg_ptr, beg_ptr, end_ptr);
        setp(beg_ptr, end_ptr);
    }

    membuf(membuf&& other) noexcept
    {
        setp(nullptr, nullptr);
        setg(nullptr, nullptr, nullptr);

        other.swap(*this);

        buf = std::move(other.buf);
        buf_size = other.buf_size;

        other.buf_size = 0;
    }

    pos_type seekoff(off_type pos, std::ios_base::seekdir seek_dir,
        std::ios_base::openmode) override
    {
        // TODO implement it in correct way
        if (seek_dir == std::ios_base::beg) {
            return 0 + pos;
        } else if (seek_dir == std::ios_base::end) {
            return buf_size + pos;
        } else {
            return egptr() - gptr();
        }
    }

    membuf(membuf const&) = delete;
    membuf& operator=(membuf const&) = delete;

    char* begin() const { return eback(); }
    size_t size() const { return buf_size; }

private:
    std::unique_ptr<char[]> buf;
    size_t buf_size;
};

} //end of namespace
