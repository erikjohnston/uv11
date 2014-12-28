#include "buffer.hh"

using namespace uvpp;

Buffer uvpp::make_buffer(char * base, unsigned int len) {
    return ::uv_buf_init(base, len);
}
Buffer uvpp::make_buffer(std::vector<char>& v) {
    return ::uv_buf_init(v.data(), v.size());
}
