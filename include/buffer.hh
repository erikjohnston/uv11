#pragma once


#include <cstdlib>
#include <string>
#include <vector>

namespace uvpp {
    class BufferView {
    public:
        using iterator = char *;
        using const_iterator = char const *;

        BufferView(char const *, std::size_t len);

        BufferView(std::vector<char> const &c) : data_(c.data()), length_(c.size()) {
        }

        BufferView(std::string const &c) : data_(c.data()), length_(c.size()) {
        }

        BufferView(BufferView const &) = delete;

        BufferView(BufferView &&) = delete;

        char const *data() const;

        std::size_t length() const;

        std::size_t size() const;

//      iterator begin();
//      iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

    private:
        char const *const data_;
        std::size_t length_;
    };
}