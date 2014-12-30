#include "loop.hh"

#include "types.hh"

using namespace uvpp;

Loop::Loop(bool use_default) : is_default(use_default) {
    if (!is_default) ::uv_loop_init(&loop);
}

Loop::~Loop() {
    if (!is_default) {
        int s = ::uv_loop_close(&loop);
        if (s < 0) throw make_error(s);
    }
}

uv_loop_t& Loop::Get() { return is_default ? *::uv_default_loop() : loop; }
uv_loop_t const& Loop::Get() const { return is_default ? *::uv_default_loop() : loop; }

Loop uvpp::default_loop(true);

int uvpp::run(Loop& loop, RunMode mode) {
    return ::uv_run(&loop.Get(), mode);
}
