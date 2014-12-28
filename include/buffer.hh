#pragma once

#include <cstdlib>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "uv.h"

namespace uvpp {
    using ::uv_buf_t;
    using Buffer = uv_buf_t;

    Buffer make_buffer(std::vector<char>&);
    Buffer make_buffer(char*, unsigned int);

    // Allow people to iterate over uv_buf_t. These are found via ADL.
    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char* begin(B& buf) {
        return buf.base;
    }

    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char const* begin(B& buf) {
        return buf.base;
    }

    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char const* cbegin(B& buf) {
        return buf.base;
    }

    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char* end(B& buf) {
        return buf.base + buf.len;
    }

    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char const* end(B& buf) {
        return buf.base + buf.len;
    }

    template<typename B, typename = typename std::enable_if<std::is_same<B, Buffer>::value>::type >
    char const* cend(B& buf) {
        return buf.base + buf.len;
    }
}


