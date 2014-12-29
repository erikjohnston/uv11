#include "handles.hh"

using namespace uvpp;

Handle::Handle(uv_handle_t* h) : handle_ptr(h){
    h->data = this;
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


bool uvpp::is_active(Handle const& handle) {
    return ::uv_is_active(&handle.GetHandle());
}

bool uvpp::is_closing(Handle const& handle) {
    return handle.is_closing();
}

void uvpp::close(Handle& handle) {
    return handle.close();
}

void uvpp::close(Handle& handle, CloseCb const& close_cb) {
    return handle.close(close_cb);
}

void uvpp::ref(Handle& handle) {
    return ::uv_ref(&handle.GetHandle());
}

void uvpp::unref(Handle& handle) {
    return ::uv_unref(&handle.GetHandle());
}

bool uvpp::has_ref(Handle const& handle) {
    return ::uv_has_ref(&handle.GetHandle());
}

Error uvpp::send_buffer_size(Handle& handle, int* value) {
    int s = ::uv_send_buffer_size(&handle.GetHandle(), value);
    return make_error(s);
}

Error uvpp::recv_buffer_size(Handle& handle, int* value) {
    int s = ::uv_recv_buffer_size(&handle.GetHandle(), value);
    return make_error(s);
}

Error uvpp::fileno(Handle const& handle, uv_os_fd_t* fd) {
    int s = ::uv_fileno(&handle.GetHandle(), fd);
    return make_error(s);
}
