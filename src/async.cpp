#include "uv11/handles/async.hh"

using namespace uv11;

Async::Async(Loop& loop, Async::Callback const& cb) : HandleBase(this), async_cb(cb) {
    ::uv_async_init(&loop.Get(), &this->Get(), [] (::uv_async_t* handle ) {
        Async* a = reinterpret_cast<Async*>(handle->data);
        auto callback = a->async_cb;
        callback(*a);
    });
}

Async::~Async() {}


Error uv11::async_send(Async& async) {
    int s = ::uv_async_send(&async.Get());

    return make_error(s);
}
