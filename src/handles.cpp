#include "uv11/requests.hh"
#include "uv11/loop.hh"
#include "uv11/handles.hh"

using namespace uv11;

Handle::Handle(uv_handle_t* h, void* data) : handle_ptr(h) {
    h->data = data;
}

Handle::~Handle() {}

uv_handle_t& Handle::GetHandle() {
    return *handle_ptr;
}

uv_handle_t const& Handle::GetHandle() const {
    return *handle_ptr;
}

void Handle::close() {
    closing = true;
    ::uv_close(&GetHandle(), [](uv_handle_t*){});
}

void Handle::close(CloseCb const& close_cb) {
    on_close = close_cb;

    closing = true;
    ::uv_close(&GetHandle(), [](uv_handle_t* handle){
        Handle* h = reinterpret_cast<Handle*>(handle->data);
        h->on_close(*h);
    });
}

bool Handle::is_closing() const {
    return closing;
}


bool uv11::is_active(Handle const& handle) {
    return ::uv_is_active(&handle.GetHandle());
}

bool uv11::is_closing(Handle const& handle) {
    return handle.is_closing();
}

void uv11::close(Handle& handle) {
    return handle.close();
}

void uv11::close(Handle& handle, CloseCb const& close_cb) {
    return handle.close(close_cb);
}

void uv11::ref(Handle& handle) {
    return ::uv_ref(&handle.GetHandle());
}

void uv11::unref(Handle& handle) {
    return ::uv_unref(&handle.GetHandle());
}

bool uv11::has_ref(Handle const& handle) {
    return ::uv_has_ref(&handle.GetHandle());
}

Error uv11::send_buffer_size(Handle& handle, int* value) {
    int s = ::uv_send_buffer_size(&handle.GetHandle(), value);
    return make_error(s);
}

Error uv11::recv_buffer_size(Handle& handle, int* value) {
    int s = ::uv_recv_buffer_size(&handle.GetHandle(), value);
    return make_error(s);
}

Error uv11::fileno(Handle const& handle, uv_os_fd_t* fd) {
    int s = ::uv_fileno(&handle.GetHandle(), fd);
    return make_error(s);
}


Poll::Poll(int fd) : Poll(default_loop, fd) {}


Poll::Poll(Loop& loop, int fd) : HandleBase(this) {
    ::uv_poll_init(&loop.Get(), &this->Get(), fd);
}

Error uv11::poll_start(Poll& poll, int events, PollCb const& poll_cb) {
    poll.poll_cb = poll_cb;
    int s = ::uv_poll_start(&poll.Get(), events, [](uv_poll_t* ptr, int status, int events) {
        Poll* p = reinterpret_cast<Poll*>(ptr->data);
        p->poll_cb(*p, make_error(status), events);
    });

    return make_error(s);
}

Error uv11::poll_stop(Poll& poll) {
    int s = ::uv_poll_stop(&poll.Get());

    poll.poll_cb = nullptr;

    return make_error(s);
}
