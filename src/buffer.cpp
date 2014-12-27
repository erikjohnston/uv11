#include "buffer_view.hh"

using namespace uvpp;

BufferView::BufferView(char const* data, std::size_t len) : data_(data), length_(len) {}

char const* BufferView::data() const { return data_; }
std::size_t BufferView::length() const { return length_; }
std::size_t BufferView::size() const { return length_; }

//BufferView::iterator BufferView::begin() { return data_; }
//BufferView::iterator BufferView::end() { return data_ + length_; }

BufferView::const_iterator BufferView::begin() const { return data_; }
BufferView::const_iterator BufferView::end() const { return data_ + length_; }

BufferView::const_iterator BufferView::cbegin() const  { return data_; }
BufferView::const_iterator BufferView::cend() const { return data_ + length_; }
