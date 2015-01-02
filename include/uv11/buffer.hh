#pragma once

#include <cstdlib>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "uv.h"

namespace uv11 {
    using ::uv_buf_t;

    struct Buffer : public uv_buf_t {
        Buffer();
        Buffer(uv_buf_t const& t);
        Buffer(uv_buf_t && t);
    };

    Buffer make_buffer(std::vector<char>&);
    Buffer make_buffer(char*, unsigned int);

    // Allow people to iterate over uv_buf_t. These are found via ADL.
    char* begin(Buffer& buf);
    char const* begin(Buffer const& buf);
    char const* cbegin(Buffer& buf);

    char* end(Buffer& buf);
    char const* end(Buffer const& buf);
    char const* cend(Buffer& buf);
}


