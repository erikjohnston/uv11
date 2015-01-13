#include "uv11/handles/timer.hh"

using namespace uv11;

Timer::Timer(Loop& loop) : HandleBase(this) {
    ::uv_timer_init(&loop.Get(), &this->Get());
}

Timer::~Timer() {}


Error uv11::timer_start(Timer& timer, Timer::Callback const& callback, uint64_t timeout, uint64_t repeat) {
    timer.callback = callback;

    int s = ::uv_timer_start(
        &timer.Get(),
        [] (uv_timer_t* t) {
            Timer* ptr = reinterpret_cast<Timer*>(t->data);
            auto c = ptr->callback;
            c(*ptr);
        },
        timeout, repeat
    );

    return make_error(s);
}

Error uv11::timer_stop(Timer& timer) {
    timer.callback = nullptr;
    int s = ::uv_timer_stop(&timer.Get());
    return make_error(s);
}
