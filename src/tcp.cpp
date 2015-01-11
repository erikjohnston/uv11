#include "uv11/streams/tcp.hh"

using namespace uv11;


Tcp::Tcp() : Tcp(default_loop) {
    Get().data = this;
}

Tcp::Tcp(Loop& loop) : StreamBase(this) {
    uv_tcp_init(&loop.Get(), &Get());
}

Tcp::~Tcp() {
    if (!is_closing()) close();
}

Error uv11::tcp_connect(ConnectRequest& req, Tcp& tcp, sockaddr const& addr, ConnectCb const& connect_cb) {
    req.connect_cb = connect_cb;
    int s = ::uv_tcp_connect(
        &req.Get(),
        &tcp.Get(),
        &addr,
        [] (uv_connect_t* req_ptr, int status) {
            ConnectRequest* r = reinterpret_cast<ConnectRequest*>(req_ptr->data);

            auto conn_cb = r->connect_cb;
            r->connect_cb = nullptr;
            conn_cb(*r, make_error(status));
        }
    );

    if (s) req.connect_cb = nullptr;

    return make_error(s);
}

Error uv11::tcp_open(Tcp & tcp, uv_os_sock_t sock) {
    return make_error(::uv_tcp_open(&tcp.Get(), sock));
}

Error uv11::tcp_nodely(Tcp & tcp, bool enable) {
    return make_error(::uv_tcp_nodelay(&tcp.Get(), enable));
}

Error uv11::tcp_keepalive(Tcp & tcp, bool enable, unsigned int delay) {
    return make_error(::uv_tcp_keepalive(&tcp.Get(), enable, delay));
}

Error uv11::tcp_simultaneous_accepts(Tcp & tcp, bool enable) {
    return make_error(::uv_tcp_simultaneous_accepts(&tcp.Get(), enable));
}

Error uv11::tcp_bind(Tcp & tcp, sockaddr const & sockaddr, unsigned int flags) {
    return make_error(::uv_tcp_bind(&tcp.Get(), &sockaddr, flags));
}

Error uv11::tcp_getsockname(Tcp const & tcp, sockaddr & sockaddr, int * len) {
    return make_error(::uv_tcp_getsockname(&tcp.Get(), &sockaddr, len));
}

Error uv11::tcp_getpeername(Tcp const & tcp, sockaddr & sockaddr, int * len) {
    return make_error(::uv_tcp_getpeername(&tcp.Get(), &sockaddr, len));
}
