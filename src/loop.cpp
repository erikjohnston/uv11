#include "loop.hh"

using namespace uvpp;

Loop::Loop(bool use_default) : is_default(use_default) {
    if (!is_default) ::uv_loop_init(&loop);
}

Loop::~Loop() {
    if (!is_default) ::uv_loop_close(&loop);
}

uv_loop_t& Loop::Get() { return loop; }
uv_loop_t const& Loop::Get() const { return loop; }

Loop uvpp::default_loop;
