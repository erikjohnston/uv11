#pragma once

#include "types.hh"

namespace uvpp {
    class Tcp : public StreamWrapper< ::uv_tcp_t> {
    public:
        Tcp() {
            ::uv_tcp_init(&Get());
        }

        ~Tcp() {
            ::uv_close(this, nullptr);
        }
    };

    class Pipe : public StreamWrapper< ::uv_pipe_t> {
    public:
        Pipe() {
            ::uv_tcp_init(&Get());
        }

        ~Pipe() {
            ::uv_close(this, nullptr);
        }
    };
}