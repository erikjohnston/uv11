#pragma once

#include "../handles.hh"
#include "../internals.hh"
#include "../loop.hh"


namespace uv11 {

    class Async : public HandleBase<uv_async_t> {
    public:
        using Callback = std::function<void(Async&)>;

        Async(Loop&, Callback const&);
        virtual ~Async();

        Callback async_cb;
    };

    Error async_send(Async&);
}
