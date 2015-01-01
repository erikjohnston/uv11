#include "uv11/buffer.hh"

using namespace uv11;

Buffer uv11::make_buffer(char * base, unsigned int len) {
    return ::uv_buf_init(base, len);
}
Buffer uv11::make_buffer(std::vector<char>& v) {
    return ::uv_buf_init(v.data(), v.size());
}
