#pragma once

#include "uv.h"

namespace uv11 {
    using ::uv_loop_t;

    using RunMode = ::uv_run_mode;

    class Loop {
    public:
        Loop(bool use_default=false);
        ~Loop();

        Loop(Loop const&) = delete;

        uv_loop_t& Get();
        uv_loop_t const& Get() const;

    private:
        uv_loop_t loop;
        bool const is_default;
    };

    extern Loop default_loop;

    int run(Loop& loop, RunMode mode);
}
