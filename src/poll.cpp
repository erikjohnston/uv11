#include "uv11/handles/poll.hh"


using namespace uv11;


Poll::Poll(int fd) : Poll(default_loop, fd) {}


Poll::Poll(Loop& loop, int fd) : HandleBase(this) {
    ::uv_poll_init(&loop.Get(), &this->Get(), fd);
}

Poll::~Poll() {}



Error uv11::poll_start(Poll& poll, int events, Poll::Callback const& poll_cb) {
    poll.poll_cb = poll_cb;
    int s = ::uv_poll_start(&poll.Get(), events, [](uv_poll_t* ptr, int status, int ev) {
        Poll* p = reinterpret_cast<Poll*>(ptr->data);
        p->poll_cb(*p, make_error(status), ev);
    });

    return make_error(s);
}

Error uv11::poll_stop(Poll& poll) {
    int s = ::uv_poll_stop(&poll.Get());

    poll.poll_cb = nullptr;

    return make_error(s);
}