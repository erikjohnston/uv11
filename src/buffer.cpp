#include "buffer_view.hh"

using namespace uvpp;

Buffer uvpp::make_buffer(std::vector<char>& v) {
    uv_buf_t buf;
    uv_buf_init(v.data(), v.size());
    return buf;
}

//BufferView::BufferView(char const* data, std::size_t len) : base(data), len(len) {}
//
//char const* BufferView::data() const { return base; }
//std::size_t BufferView::length() const { return len; }
//std::size_t BufferView::size() const { return len; }
//
//BufferView::iterator BufferView::begin() { return data_; }
//BufferView::iterator BufferView::end() { return data_ + length_; }

Buffer::Buffer() {}

Buffer::Buffer(char * b, std::size_t l) : base(b), len(l) {}
Buffer::Buffer(std::vector<char>& v) : base(v.data()), len(v.size()) {}
Buffer::Buffer(std::string& str) : base(str.data()), len(str.size()) {}

char* Buffer::begin() const { return base; }
char const* Buffer::begin() const { return base; }
char const* Buffer::cbegin() const  { return base; }

char* Buffer::end() { return base + len; }
char const* Buffer::end() const { return base + len; }
char const* Buffer::cend() const { return base + len; }
