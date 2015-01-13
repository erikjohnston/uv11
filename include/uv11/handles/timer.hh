#pragma once

#include "../handles.hh"
#include "../internals.hh"
#include "../loop.hh"


namespace uv11 {

    class Timer : public HandleBase<uv_timer_t> {
    public:
        using Callback = std::function<void(Timer&)>;

        Timer(Loop& = uv11::default_loop);
        virtual ~Timer();

        Callback callback;
    };

    Error timer_start(Timer&, Timer::Callback const&, uint64_t timeout, uint64_t repeat);
    Error timer_stop(Timer&);
}
