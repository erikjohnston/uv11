#pragma once

#include "types.hh"

namespace uvpp {
    class Udp : public HandleWrapper< ::uv_udp_t> {
    public:
        Udp() {
            ::uv_udp_init(&Get());
        }

        ~Udp() {
            ::uv_close(this, nullptr);
        }
    };
}