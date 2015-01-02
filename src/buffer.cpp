#include "uv11/buffer.hh"

using namespace uv11;

Buffer uv11::make_buffer(char * base, unsigned int len) {
    return ::uv_buf_init(base, len);
}
Buffer uv11::make_buffer(std::vector<char>& v) {
    return ::uv_buf_init(v.data(), v.size());
}

Buffer::Buffer() {}
Buffer::Buffer(uv_buf_t const &t) : uv_buf_t(t) {}
Buffer::Buffer(uv_buf_t &&t) : uv_buf_t(std::move(t)) {}

char* uv11::begin(Buffer& buf) {
    return buf.base;
}

char const* uv11::begin(Buffer const& buf) {
    return buf.base;
}

char const* uv11::cbegin(Buffer& buf) {
    return buf.base;
}

char* uv11::end(Buffer& buf) {
    return buf.base + buf.len;
}

char const* uv11::end(Buffer const& buf) {
    return buf.base + buf.len;
}

char const* uv11::cend(Buffer& buf) {
    return buf.base + buf.len;
}
