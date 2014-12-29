#pragma once

#include "uv.h"

namespace uvpp {
    using ::uv_loop_t;

    class Loop {
    public:
        Loop(bool use_default=false);
        ~Loop();

        Loop(Loop const&) = delete;

        uv_loop_t& Get();
        uv_loop_t const& Get() const;

    private:
        uv_loop_t loop;
        bool is_default;
    };

    extern Loop default_loop;
}
