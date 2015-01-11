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
