#pragma once

#include "uv.h"

namespace uvpp {
    using ::uv_loop_t;

    class Loop {
    public:
        Loop();

        uv_loop_t& Get();
        uv_loop_t const& Get() const;

        static Loop get_default();

    protected:
        Loop(uv_loop_t*);

    private:
        uv_loop_t* loop;
    };
}
