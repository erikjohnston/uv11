#pragma once

#include <cstdlib>
#include <iterator>
#include <string>
#include <vector>

#include "uv.h"

namespace uvpp {
    using ::uv_buf_t;
    // using Buffer = uv_buf_t;

    class Buffer : public uv_buf_t {
    public:
        Buffer();
        Buffer(char*, std::size_t);
        Buffer(std::vector<char>&);
        Buffer(std::string&);

        char* begin();
        char const * begin() const;
        char const * cbegin() const;

        char* end();
        char const * end() const;
        char const * cend() const;
    };

    static_assert(sizeof(Buffer) == sizeof(uv_buf_t), "Buffer not same size as uv_buf_t");
}
