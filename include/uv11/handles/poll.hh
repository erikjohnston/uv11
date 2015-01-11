#pragma once

#include "../handles.hh"
#include "../internals.hh"
#include "../loop.hh"


namespace uv11 {

    class Poll : public HandleBase<uv_poll_t> {
    public:
        using Callback = std::function<void(Poll&, Error, int events)>;

        Poll(int fd);
        Poll(Loop&, int fd);
        virtual ~Poll();

        Callback poll_cb;
    };


    Error poll_start(Poll&, int events, Poll::Callback const&);
    Error poll_stop(Poll&);
}
